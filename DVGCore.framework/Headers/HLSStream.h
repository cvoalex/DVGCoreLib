//  Copyright (c) 2017 DENIVIP Group. All rights reserved.

#ifndef HLSChunk_h
#define HLSChunk_h

#import <AVFoundation/AVFoundation.h>

typedef enum {
    ChunkDataType_FullChunkNormal,
    ChunkDataType_FullChunkLL,
    ChunkDataType_FullChunkLLUploadFailed,
    ChunkDataType_StreamHlsFinal
} ChunkDataType;

#define kDVGUploadLastVALS 4
// Difference between recorded stream duration and duration of part of stream uploaded on server
static NSString* _Nonnull const kDVGStreamerStatRtDelayStream = @"kDVGStreamerStatRtDelayStream";

// Difference in timestamps between most recent frame from physical camera and last frame in this chunk
static NSString* _Nonnull const kDVGStreamerStatRtDelayLastChunk = @"kDVGStreamerStatRtDelayLastChunk";
// Upload bitrate of last chunk (size/time to upload)
static NSString* _Nonnull const kDVGStreamerStatRtBitrateLastChunk = @"kDVGStreamerStatRtBitrateLastChunk";

// Current level of stream downgrade (0 - no downgrade, 1 - mid downgrade, 2 - full downgrade).
// Downgrades are made automatically to minimize upload sizes when upload delays going up
static NSString* _Nonnull const kDVGStreamerStatRtQualityLevel = @"kDVGStreamerStatRtQualityLevel";

// same as kDVGStreamerStatRtDelayLastChunk but for most recent Microchunk (LL-streaming)
static NSString* _Nonnull const kDVGStreamerStatRtDelayMicro = @"kDVGStreamerStatRtDelayMicro";
// Timestamp of the last frame in most recent Microchunk (LL-streaming)
static NSString* _Nonnull const kDVGStreamerStatRtStampMicro = @"kDVGStreamerStatRtStampMicro";


static NSString* _Nonnull const kDVGStreamerStatsActiveDnChunk = @"kDVGStreamerStatsActiveDnChunk";

@interface HLSChunk : NSObject
@property (strong) NSString* _Nonnull streamId;
@property (assign) NSInteger recsetId;
@property (assign) NSInteger sequence;

@property (strong) NSString* _Nonnull name;
//@property (strong) NSURL* _Nullable filePath;
@property (assign) NSInteger chunkSizeTS;// Full chunk size, ignoring LowLat microchunking
@property (assign) NSInteger chunkSizeLL;// For LowLat chunks contain microchunks size, otherwise same as chunkSizeTS
@property (assign) double vduration;
@property (assign) double aduration;
@property (assign) double fps;

@property (assign) double vPtsIn;
@property (assign) double vPtsOut;

@property (assign) double aPtsIn;
@property (assign) double aPtsOut;

@property (assign) NSInteger chunkLowLatRefId;
@property (assign) double chunkUploadDeadline;
@property (assign) NSInteger chunkPatStart;
@property (assign) NSInteger chunkPatSize;
@property (assign) double bentoLastCts;
@property (assign) double localPtsTs;

@property (strong) NSString* _Nullable dbgLlHash;
@property (strong) NSString* _Nullable dbgLlHashPrev;
@end


@interface StreamMetricsInfo : NSObject
@property (assign) double bitrate;
@property (assign) NSInteger width;
@property (assign) NSInteger height;
@property (assign) NSInteger seqInMuxing;
@property (assign) NSInteger seqInMuxingDone;
    
@property (assign) double vduration;
@property (assign) double aduration;
@property (assign) double uduration_full;
@property (assign) double uduration_rt;

@property (assign) double lastLLChunk_bitrate;
@property (assign) double lastLLChunk_rtDelay;

@property (assign) double lastLLChunk_microRtDelay;
@end

@class HLSStream;
@class HLSRecSettings;
typedef BOOL (^OnStreamBindingDownBlock)(HLSStream* _Nonnull stream, NSString * _Nullable error);
typedef BOOL (^OnStreamChunkGenerationReadyBlock)(HLSStream* _Nonnull stream);
typedef BOOL (^OnStreamChunkGenerationDoneBlock)(HLSStream* _Nonnull stream, HLSChunk* _Nonnull chunk, NSData* _Nonnull chunkData, ChunkDataType chunkType);
typedef BOOL (^OnStreamChunkLLUploadDoneBlock)(HLSStream* _Nonnull stream, HLSChunk* _Nonnull chunk, double time, double rtDelay, NSInteger size);
typedef BOOL (^OnStreamDynDowngradeBlock)(HLSStream* _Nonnull stream, HLSChunk* _Nonnull chunk, double rtDelay, BOOL isDowngrade);
typedef BOOL (^OnStreamStatsBlock)(HLSStream* _Nonnull stream, NSDictionary * _Nullable stats);

@interface HLSStream : NSObject
@property (strong, nonatomic) OnStreamBindingDownBlock _Nullable onStreamBindingDown;
@property (strong, nonatomic) OnStreamChunkGenerationReadyBlock _Nullable onChunkReady2mux;
@property (strong, nonatomic) OnStreamChunkGenerationDoneBlock _Nullable onChunkGenerated;
@property (strong, nonatomic) OnStreamChunkLLUploadDoneBlock _Nullable onChunkLLUploaded;
@property (strong, nonatomic) OnStreamDynDowngradeBlock _Nullable onStreamDynDowngrade;
@property (strong, nonatomic) OnStreamStatsBlock _Nullable onStreamStatsUpdated;

- (instancetype _Nonnull )initWithStreamId:(NSString* _Nonnull)streamId path:(NSString* _Nonnull)path;
- (void)hwencPrepareBinding;
- (void)hwencResetBinding;
- (void)hwencCancelAll;
- (BOOL)hwencBindToHardwareWithSettings:(HLSRecSettings* _Nonnull)settings;
- (BOOL)hwencIsBindedToHardware;
- (BOOL)hwencUnbind;
- (void)hwencEncodeVideo:(CMSampleBufferRef _Nonnull )sampleBuffer;
- (void)hwencEncodeAudio:(CMSampleBufferRef _Nonnull )sampleBuffer;
- (StreamMetricsInfo*_Nonnull)hwencGetEffectiveMetrics;

- (NSString* _Nullable )getStreamId NS_SWIFT_NAME(getStreamId());
- (HLSRecSettings* _Nullable )getStreamSettings NS_SWIFT_NAME(getStreamSettings());
- (double)getStreamDuration NS_SWIFT_NAME(getStreamDuration());
- (NSArray<HLSChunk*>* _Nonnull )getStreamChunks NS_SWIFT_NAME(getStreamChunks());
- (NSString* _Nullable )getStreamHLSName NS_SWIFT_NAME(getStreamHLSName());
- (NSString* _Nullable )getStreamHLS:(BOOL)finalState NS_SWIFT_NAME(getStreamHLS(_:));
+ (NSString* _Nullable )getMasterHLSForStreams:(NSArray*_Nonnull)streams NS_SWIFT_NAME(getMasterHLSForStreams(_:));
@end



#endif
