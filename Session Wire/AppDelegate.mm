//
//  AppDelegate.m
//  Session Wire
//
//  Created by Carson Fee on 5/11/15.
//  Copyright (c) 2015 Carson Fee. All rights reserved.
//

#import "AppDelegate.h"


@interface AppDelegate ()
@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

SWSocket *pSockets;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    
    //Create the socket layer immediately
    pSockets = new SWSocket();
    pSockets->InitSWSocket();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    [_window close];
    pSockets->Exit();
    delete pSockets;
    pSockets = NULL;
}

@end
