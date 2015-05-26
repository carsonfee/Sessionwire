//
//  QTCaptureView+VideoView.m
//  Session Wire
//
//  Created by Carson Fee on 5/11/15.
//  Copyright (c) 2015 Carson Fee. All rights reserved.
//

#import "VideoView.h"


extern ReaderWriterQueue<SWPacket> GOutPacketQueue;
extern ReaderWriterQueue<SWPacket> GInPacketQueue;

@implementation VideoView
SWPacket testPacket;
AVCaptureSession *session;
AVCaptureDeviceInput *input;

- (void)viewDidLoad
{
    [super viewDidLoad];
    session = [[AVCaptureSession alloc] init];
    session.sessionPreset = AVCaptureSessionPresetMedium;
    
    AVCaptureVideoPreviewLayer *captureVideoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:session];
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    
    NSError *error = nil;
    input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
    if (!input) {
        NSLog(@"ERROR: trying to open camera: %@", error);
    }
    
    [session addInput:input];
    [self.view setWantsLayer:YES];
    captureVideoPreviewLayer.frame = self.view.bounds;
    [self.view.layer addSublayer:captureVideoPreviewLayer];
}

- (void)viewDidAppear
{
    [super viewDidAppear];
    [session startRunning];
}

- (void)viewWillDisappear
{
    [super viewWillDisappear];
    [session stopRunning];
}

- (IBAction)TestSendSocket:(id)sender
{
    testPacket.buffer[0] = 'a';
    bool success = GOutPacketQueue.enqueue(testPacket);
    std::cout << "Enque did " << success << std::endl;
}

- (void) dealloc
{
    session = nil;
    input = nil;
}
@end


