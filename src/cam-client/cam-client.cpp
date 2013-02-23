// vim:shiftwidth=4:tabstop=4:expandtab
//
// cam-capture.cpp - Dumps video to a file
//
// Author:
//   Kevin Godby <godbyk@gmail.com>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include "ClientSocket.h"
#include "SocketException.h"
#include <sys/time.h>       // for timestamp
#include <time.h>           // for timestamp
#include "blob.h"
#include "blobresult.h"

using namespace std;

bool   DEBUG = true;        // print debug spew?
bool   CONNECT = true;     // connect to server?
string server_name = "";
int    server_port = 2129;

// Definitions
double getTimeStamp();
bool getYCoords(IplImage* image, int &yCoordA, int &yCoordB);
CBlobResult sortBlobs(CBlobResult blobs);
void drawYoffsets(IplImage* image, const CBlob &A, const CBlob &B);

// Code!
int main(int argc, char** argv) 
{
    double start_time = getTimeStamp();
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed);
    CvCapture* capture = NULL;

    // Check command-line switches
    /*
    if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
        capture = cvCaptureFromCAM(argc == 2 ? argv[1][0] - '0' : 0);
    else if (argc == 2)
        capture = cvCaptureFromFile(argv[1]); 
    */
    capture = cvCaptureFromCAM(0);

    for (int i = 0; i < argc; i++) {
        if (argv[i] == "--server") {
            if (i+1 >= argc) {
                string server_name = argv[i+1];
                CONNECT = true;
            } else {
                std::cerr << "ERROR: Must specify server after --server paramter!" << std::endl;
                exit(1);
            }
        }
    }
            

    // If we can't get video, die
    if (!capture) {
        cerr << "ERROR: Couldn't use capture device." << endl;
        return 1;
    }
    
    // Create the window and add the trackbar
    cvNamedWindow("Video", CV_WINDOW_AUTOSIZE);

    // Find the framerate of the capture device
    double framerate = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    framerate = 30.0;       // hard-coded because my camera or v4l doesn't support the FPS query above

    // Save everything to an AVI file
    //CvVideoWriter* videoWriter = cvCreateVideoWriter("output_video.avi", CV_FOURCC('P', 'I', 'M', '1'), framerate, cvSize(640,480));
     
    IplImage* image = NULL;
    IplImage* clean = NULL;
 
    // Connect to server or die
    bool isConnected = false;
    ClientSocket client_socket;
    
    if (CONNECT) {
        try {
            client_socket.connect(server_name, server_port);
            isConnected = true;
        } catch (SocketException& e) {
            cerr << "ERROR: Caught exception: " << e.description() << endl;
            isConnected = false;
            return 1;
        }
    }

    string reply = "";      // reply from server
    float yCoordA = 0.0;
    float yCoordB = 0.0;
    double timestamp = getTimeStamp() - start_time;
    int frame = 0;
    CvFont font;
    
    // Main loop
    while (cvGrabFrame(capture)) {
        // Grab the iamge from the capture device (camera or avi file)
        image = cvRetrieveFrame(capture);

        // Increment frame counter
        frame++;
        
        clean = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
        cvCvtColor(image, clean, CV_RGB2GRAY);
        
        // Get rid of any noise (and IR emitters above C4)
        cvThreshold(clean, clean, 32, 255, CV_THRESH_BINARY);
        int dia = 6;
        IplConvKernel* disc_strel = cvCreateStructuringElementEx(dia, dia, dia/2, dia/2, CV_SHAPE_ELLIPSE, NULL);
        cvMorphologyEx(clean, clean, disc_strel, NULL, CV_MOP_OPEN);
        cvDilate(clean, clean, NULL, 2);
        cvReleaseStructuringElement(&disc_strel);
        
        CBlobResult blobs = CBlobResult(clean, NULL, 100, true);
        blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, 5000 );// area <150
        blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 100 );// area <150
        CBlob blob;
        //int centerX, centerY, area;
        if (blobs.GetNumBlobs() != 3) {
            cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.75, 0.75, 0, 2);
            //CvSize text_size;
            char hud[50];
            sprintf(hud, "[%05d]  Wrong number of blobs: %d", frame, blobs.GetNumBlobs());
            cvPutText(image, hud, cvPoint(5, image->height - 10), &font, cvScalar(0, 255, 255));
            
            cvShowImage("Video", image);
            if (CONNECT && isConnected) {
                timestamp = getTimeStamp() - start_time;
                try {
                    // Keep sending until we get an 'OK' response
                    //while (reply != "OK") {
                        if (DEBUG) {
                            cout << "DEBUG: Sending to " << server_name << ":" << server_port << ":" << endl;
                            cout << "@" << timestamp << "\t" << yCoordA << "\t" << yCoordB << "!" << endl;
                        }
                        client_socket << "@" << timestamp << "\t" << yCoordA << "\t" << yCoordB << "!\n";
                        client_socket >> reply;
                        if (DEBUG)
                            cerr << "DEBUG: Received response from server: " << reply << endl;

                    //}
                        reply = "";
                } catch (SocketException& e) {
                    cerr << "ERROR: Couldn't send to " << 
                        server_name << ":" << server_port << ":\n\t" << e.description() << endl;
                }
            }

            cvWaitKey(20);
            cvReleaseImage(&clean);
            continue;
        }

        // sort in order from left to right
        //blobs = sortBlobs(blobs);

        CBlob BlobLeft, BlobCenter, BlobRight;
        blobs.GetNthBlob(CBlobGetXCenter(), 0, BlobRight);
        blobs.GetNthBlob(CBlobGetXCenter(), 1, BlobCenter);
        blobs.GetNthBlob(CBlobGetXCenter(), 2, BlobLeft);

        
        // color the blobs' bounding boxes
        // Left = red
        cvRectangle(image, cvPoint((int)BlobLeft.MinX(), (int)BlobLeft.MinY()), 
                cvPoint((int)BlobLeft.MaxX(), (int)BlobLeft.MaxY()), 
                CV_RGB(255, 0, 0), 1, 8, 0);
        // Center = blue
        cvRectangle(image, cvPoint((int)BlobCenter.MinX(), (int)BlobCenter.MinY()), 
                cvPoint((int)BlobCenter.MaxX(), (int)BlobCenter.MaxY()), 
                CV_RGB(0, 0, 255), 1, 8, 0);
        // Right = green
        cvRectangle(image, cvPoint((int)BlobRight.MinX(), (int)BlobRight.MinY()), 
                cvPoint((int)BlobRight.MaxX(), (int)BlobRight.MaxY()), 
                CV_RGB(0, 255, 0), 1, 8, 0);

        // calculate delta-Y's and draw some dimension lines on the image
        drawYoffsets(image, BlobLeft, BlobCenter);
        drawYoffsets(image, BlobCenter, BlobRight);
        
        CBlobGetYCenter getYCenter;
        yCoordA = -(getYCenter(BlobLeft) - getYCenter(BlobCenter)) / (image->height/2);
        yCoordB = -(getYCenter(BlobRight) - getYCenter(BlobCenter)) / (image->height/2);
        
        // Print HUD on image
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.75, 0.75, 0, 2);
        //CvSize text_size;
        char hud[50];
        sprintf(hud, "[%05d]  L: %01.3f  R: %01.3f", frame, yCoordA, yCoordB);
        cvPutText(image, hud, cvPoint(5, image->height - 10), &font, cvScalar(0, 255, 255));
        
        // Display the images
        cvShowImage("Video", image);

        // Send data to server
        timestamp = getTimeStamp() - start_time;
        string reply = "";
        if (CONNECT && isConnected) {
            try {
                // Keep sending until we get an 'OK' response
                //while (reply != "OK") {
                    if (DEBUG) {
                        cout << "DEBUG: Sending to " << server_name << ":" << server_port << ":" << endl;
                        cout << "@" << timestamp << "\t" << yCoordA << "\t" << yCoordB << "!" << endl;
                    }
                    client_socket << "@" << timestamp << "\t" << yCoordA << "\t" << yCoordB << "!\n";
                    client_socket >> reply;
                    if (DEBUG)
                        cerr << "DEBUG: Received response from server: " << reply << endl;

                //}
                    reply = "";
            } catch (SocketException& e) {
                cerr << "ERROR: Couldn't send to " << 
                    server_name << ":" << server_port << ":\n\t" << e.description() << endl;
            }
        }

        // Save image to avi file
        //cvWriteFrame(videoWriter, image);

        // Release temp images
        cvReleaseImage(&clean);
        
        // Press 'q' to stop the program
        if (cvWaitKey(20) == 'q')
            break;
    }

    if (DEBUG)
        cerr << "DEBUG: Exiting..." << endl;

    cvReleaseCapture(&capture);
    cvDestroyWindow("Video");

    return 0;
}

double getTimeStamp()
{
    // Returns the elapsed time since the program started (in microsec)
    struct timeval tval;
    struct timezone tzone;
    gettimeofday(&tval, &tzone);
    return (double)tval.tv_sec * 1000000.0 + (double)tval.tv_usec;
}

bool getYCoords(IplImage* image, int &yCoordA, int &yCoordB)
{
    return true;
}

CBlobResult sortBlobs(CBlobResult blobs)
{
    // Uses a modified bubble sort
    CBlob tmp;
    bool done = false;

    for (int i = blobs.GetNumBlobs() - 1; i > 0; i--) {
        done = true;
        for (int j = 0; i > j; j++) {
            double centerA = (blobs.GetBlob(j).MinX() + blobs.GetBlob(j).MaxX()) / 4;
            double centerB = (blobs.GetBlob(j+1).MinX() + blobs.GetBlob(j+1).MaxX()) / 4;
            if (centerA > centerB) {
                done = false;
                tmp = blobs.GetBlob(j);
                blobs.GetBlob(j) = blobs.GetBlob(j+1);
                blobs.GetBlob(j+1) = tmp;
            }
        }
        //out this block when flag is true
        //i.e. inner loop performed no swaps, so the list is already sorted
        if (done)
            break;
    }

    return blobs;
}

void drawYoffsets(IplImage* image, const CBlob &A, const CBlob &B)
{
    int AcenterX = (int)((A.MinX() + A.MaxX()) / 2);
    int AcenterY = (int)((A.MinY() + A.MaxY()) / 2);
    int BcenterX = (int)((B.MinX() + B.MaxX()) / 2);
    int BcenterY = (int)((B.MinY() + B.MaxY()) / 2);
    int midpointX = (int)((AcenterX + BcenterX) / 2);
    
    // draw extensions
    cvLine(image, cvPoint(midpointX - 3, AcenterY), 
            cvPoint(midpointX + 3, AcenterY), CV_RGB(255,255,0));
    cvLine(image, cvPoint(midpointX - 3, BcenterY), 
            cvPoint(midpointX + 3, BcenterY), CV_RGB(255,255,0));

    // draw dimension line
    cvLine(image, cvPoint(midpointX, AcenterY), 
            cvPoint(midpointX, BcenterY), CV_RGB(255,255,0));
}

