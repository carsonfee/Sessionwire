//
//  QTCaptureView+VideoView.h
//  Session Wire
//
//  Created by Carson Fee on 5/11/15.
//  Copyright (c) 2015 Carson Fee. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <AVFoundation/AVFoundation.h>
#include "SWSockets.h"
#include "readwriterqueue.h"

@interface VideoView : NSViewController {
}
@property (nonatomic, retain) NSView* vidView;
@end
