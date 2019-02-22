//  Copyright (c) 2017 DENIVIP Group. All rights reserved.

#ifndef HLSRecSettings_h
#define HLSRecSettings_h
#import <UIKit/UIKit.h>

static int const kVideoChunkMaxBufferSize = 0;
static int const kVideoStreamSpsSet = 0x01;
static int const kVideoStreamFlushReq = 0x02;
static int const kVideoStreamIFrameSet = 0x04;
static int const kVideoStreamVideoData = 0x08;
static int const kVideoStreamAudioData = 0x10;
static int const kVideoMaxUnsentDataSize = 30*1000000;
static int const kVideoMaxMuxWaitBeforeDrops = 1;

static int const kVideoStreamLowLatAdditionalFakeHLSChunks = 2;
static CGFloat const kVideoStreamLowLatSamplesSec = 0.15;
static CGFloat const kVideoStreamLowLatMaxRTDelaySec = 1.2;
static int const kVideoStreamLowLatQualityDowngradeGrace = 3;// regarding HLS chunks
static int const kVideoStreamLowLatQualityRestoreGrace = 3*10;// regarding HLS chunks
static int const kVideoStreamLowLatHLSUploadGrace = 3;

static CGFloat const kVideoHTTPUploadTimeoutDeadline = 5;
static int const kVideoHTTPUploadMaxConcurrent = 4;
static int const kVideoHTTPUploadTimeout = 30;
static int const kVideoStreamUploadTimeoutAddition = 3;
//static NSURLSessionMultipathServiceType kDVGStreamerMultipathServiceType = NSURLSessionMultipathServiceTypeHandover;
// Fraction of chunk duration to delay for prefetch
static double const kVideoStreamChunksPrepushFrac = 0.7;

static NSString* const kVideoStreamLowLatUplURLBase =
    @"http://ec2-18-213-85-167.compute-1.amazonaws.com:3000/";
    //@"http://LHLS-LB-469453332.us-east-1.elb.amazonaws.com/";
    //@"http://ec2-54-159-151-197.compute-1.amazonaws.com:3000/";

static NSString* const kVideoStreamLowLatUplFileTempl = @"{url_base}uploadMedia?playlist={name}&chunk={chunk}&frame={frame}&tm={stamp}";
static NSString* const kVideoStreamLowLatUplStreamTempl = @"{url_base}streamMedia?playlist={name}&chunk={chunk}";
static NSString* const kVideoStreamLowLatUplFinishTempl = @"{url_base}streamMedia?playlist={name}&chunk={chunk}";
static NSString* const kVideoStreamLowLatDwnChunkTempl = @"/getChunk?playlist={name}&chunk={chunk}";
static NSString* const kVideoStreamLowLatDwnMetaTempl = @"{url_base}getMeta?playlist={name}";
static NSString* const kVideoStreamLowLatUplMetaTempl = @"{url_base}setMeta?playlist={name}&meta={meta}&tm={stamp}";

typedef enum {
    HLSRecFPS_15,
    HLSRecFPS_24,
    HLSRecFPS_30,
    HLSRecFPS_60,
    
    HLSRecFPS_MAX
} HLSRecFPS;
#define kHLSRecFPSList {15, 24, 30, 60}
//#define kHLSRecFPSList {15, 15, 15, 15}// DBG

typedef enum {
    HLSRecDuration_003,
    HLSRecDuration_005,
    HLSRecDuration_007,

    HLSRecDuration_01,
    HLSRecDuration_02,
    HLSRecDuration_03,
    HLSRecDuration_05,
    HLSRecDuration_08,
    HLSRecDuration_15,
    
    HLSRecDuration_MAX
} HLSRecDuration;
#define kHLSRecDuration_IframesPerChunkList  {  1,   1,   2,   1,     1,     1,     1,     4,    15}
#define kHLSRecDuration_IframesPerSecondList {3.0, 2.0, 3.0, 1.0, 0.500, 0.329, 0.199, 0.500, 1.000}
#define kHLSRecDuration_HardSecondsLimitList {0.3, 0.5, 0.7, 1.0, 2.000, 3.000, 5.000, 8.000, 15.00}

typedef enum {
    HLSRecPreset_352x288,
    HLSRecPreset_640x480,
    HLSRecPreset_1280x720,
    HLSRecPreset_1920x1080,
    HLSRecPreset_3840x2160,
    
    HLSRecPreset_MAX
} HLSRecPreset;
// https://teradek.com/blogs/articles/what-is-the-optimal-bitrate-for-your-resolution
// https://support.google.com/youtube/answer/2853702?hl=en
// #define kHLSRecPreset_AvgBitrate {400000, 1000000, 2000000, 4000000, 8000000 }
//#define kHLSRecPreset_AvgBitrate {400000,  800000, 1000000, 2000000, 4000000 }
#define kHLSRecPreset_AvgBitrate {400000,  600000, 1200000, 1500000, 2000000 }

typedef enum {
    HLSRecFormat_TS_H264,
    HLSRecFormat_TS_HEVC,
    HLSRecFormat_TS_H264_LOWLAT,
    HLSRecFormat_TS_HEVC_LOWLAT,
    
    HLSRecFormat_FMP4_H264,
    HLSRecFormat_FMP4_HEVC,
    
    HLSRecFormat_MAX
} HLSRecFormat;

//typedef enum {
//    HLSRecStreams_FULL,
//    HLSRecStreams_FnH,
//
//    HLSRecStreams_MAX
//} HLSRecStreams;

@interface HLSRecSettings : NSObject
@property (assign) NSInteger recsetId;// Internal value for proper settings change detection, do not set/alter manually
//@property (assign) HLSRecStreams streamsBreakdown;// Deprecated

@property (assign) double videoDownsamplingFactor;

@property (assign) HLSRecDuration videoChunkDuration;
@property (assign) HLSRecFPS videoFPS;
@property (assign) HLSRecPreset videoPreset;
@property (assign) HLSRecFormat chunkFormat;
@property (strong) NSString* playListName;

-(void)resetToDefaults;
-(BOOL)compareRecSetWith:(HLSRecSettings*)another;
-(NSString*)toString;

+ (NSInteger)getRealFPS:(HLSRecFPS)fpsID;
+ (NSInteger)getIframesPerChunk:(HLSRecDuration)durationID;
+ (double)getIframesPerSec:(HLSRecDuration)durationID;
+ (double)getDurationChunkSec:(HLSRecDuration)durationID;
+ (NSString*)getRealPresetAVName:(HLSRecPreset)presetID;
+ (CGSize)getRealPresetSize:(HLSRecPreset)presetID;
+ (NSInteger)getPresetAvgBitrate:(HLSRecPreset)presetID;
+ (double*)getPTSInitialsForFormat:(HLSRecFormat)chunkFormat;
//+ (NSArray*)getStreamsMulfac:(HLSRecStreams)streamsBreakdown;
+ (NSArray*)getLowLatDinamicDownsampling:(HLSRecFormat)chunkFormat;

+ (BOOL)isRecSettingsValid:(HLSRecSettings*)settings forFrontCamera:(BOOL)forFrontCamera;
+ (id)getDeviceFormatForRecSettings:(HLSRecSettings*)settings forFrontCamera:(BOOL)forFrontCamera;
@end


#endif
