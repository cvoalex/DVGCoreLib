//  Copyright (c) 2017 DENIVIP Group. All rights reserved.
//

#ifndef H264HwEncoderImpl_h
#define H264HwEncoderImpl_h


#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "HLSRecSettings.h"

#define kAACSamplesPerFrame 1024
#define kAACFrequency 44100
#define kAACFrequencyAdtsId 4

@protocol H264HwEncoderImplDelegate <NSObject>
- (void)hwencOnStartEncoder;
- (void)hwencOnStopEncoder;
- (BOOL)hwencOnBeforeNextFrame:(BOOL)isIFrame nextPTS:(CMTime)nextPTS;

- (void)hwencOnSpsPps:(NSData* _Nonnull)spspps pts:(CMTime)pts index:(int)idx;
- (void)hwencEncodedVideoData:(NSData* _Nonnull)data pts:(CMTime)pts isKeyFrame:(BOOL)isKeyFrame;
- (void)hwencEncodedAudioData:(NSData* _Nonnull)data pts:(CMTime)pts duration:(CMTime)duration;
@end


@interface H264HwEncoderImpl : NSObject

- (instancetype)init;
- (BOOL)setupEncodingForStream:(HLSRecSettings* _Nonnull)recsettings vs:(NSDictionary*)vs as:(NSDictionary*)as;
- (BOOL)encodeVideo:(CMSampleBufferRef _Nonnull)sampleBuffer;
- (BOOL)encodeAudio:(CMSampleBufferRef _Nonnull)sampleBuffer;
- (void)stopEncoding;

- (int)iframeCount;
- (void)iframeCountReset;
- (void)iframeForceRequest;
- (void)setPresentationRefTime:(CMTime)reftime;
- (BOOL)isSampleBufferContainIFrame:(CMSampleBufferRef _Nonnull)sampleBuffer;
- (int)videoBandwith;
- (CGSize)videoResolution;

+ (UIImage * _Nullable)imageWithSampleBuffer:(CMSampleBufferRef _Nonnull)sampleBuffer;
+ (BOOL)isHEVCSupported;

@property (weak, nonatomic) id<H264HwEncoderImplDelegate> _Nullable delegate;
@property (strong) NSDictionary* _Nonnull audioSettings;
@property (strong) NSDictionary* _Nonnull videoSettings;
@property (strong) HLSRecSettings* _Nonnull streamSettings;
@property (strong) NSString* _Nullable error;
@end


#endif
