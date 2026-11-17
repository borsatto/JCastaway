#import "JohnnyCastawayView.h"
#import <pthread.h>
#include "graphics.h"
#include "resource.h"
#include "respath.h"
#include "sound.h"
#include "story.h"

// Forward declaration of the C engine hook
extern void (*jcFrameHook)(const uint8_t *rgbaPixels, int width, int height, int pitch);
extern volatile int jcEngineStopRequested;

@implementation JohnnyCastawayView {
    uint32_t _pixelBuffer[640 * 480];
    pthread_mutex_t _pixelLock;
    pthread_t _engineThread;
    BOOL _engineRunning;
}

static JohnnyCastawayView *gActiveView = nil;

static void onEngineFrame(const uint8_t *rgbaPixels, int width, int height, int pitch)
{
    if (gActiveView) {
        [gActiveView updateFrameWithPixels:rgbaPixels width:width height:height pitch:pitch];
    }
}

static void *engineThreadFunc(void *arg)
{
    @autoreleasepool {
        // Setup asset path in user Library or Bundle resources
        NSBundle *bundle = [NSBundle bundleForClass:[JohnnyCastawayView class]];
        NSString *resourcePath = [bundle resourcePath];
        if (resourcePath) {
            setAssetDir([resourcePath UTF8String]);
        }

        if (checkAssetsAvailable()) {
            graphicsInit();
            soundInit();
            storyPlay();
            soundEnd();
            graphicsEnd();
        }
    }
    return NULL;
}

- (instancetype)initWithFrame:(NSRect)frame isPreview:(BOOL)isPreview
{
    self = [super initWithFrame:frame isPreview:isPreview];
    if (self) {
        [self setAnimationTimeInterval:1.0 / 20.0];
        pthread_mutex_init(&_pixelLock, NULL);
        memset(_pixelBuffer, 0, sizeof(_pixelBuffer));
        gActiveView = self;
        jcFrameHook = onEngineFrame;
    }
    return self;
}

- (void)dealloc
{
    [self stopAnimation];
    pthread_mutex_destroy(&_pixelLock);
    if (gActiveView == self) {
        gActiveView = nil;
        jcFrameHook = NULL;
    }
}

- (void)startAnimation
{
    [super startAnimation];
    if (!_engineRunning) {
        _engineRunning = YES;
        jcEngineStopRequested = 0;
        pthread_create(&_engineThread, NULL, engineThreadFunc, (__bridge void *)self);
    }
}

- (void)stopAnimation
{
    [super stopAnimation];
    if (_engineRunning) {
        jcEngineStopRequested = 1;
        _engineRunning = NO;
        pthread_join(_engineThread, NULL);
    }
}

- (void)updateFrameWithPixels:(const uint8_t *)pixels width:(int)w height:(int)h pitch:(int)pitch
{
    pthread_mutex_lock(&_pixelLock);
    for (int y = 0; y < 480 && y < h; y++) {
        memcpy(&_pixelBuffer[y * 640], pixels + (y * pitch), 640 * sizeof(uint32_t));
    }
    pthread_mutex_unlock(&_pixelLock);
    dispatch_async(dispatch_get_main_queue(), ^{
        [self setNeedsDisplay:YES];
    });
}

- (void)animateOneFrame
{
    [self setNeedsDisplay:YES];
}

- (BOOL)hasConfigureSheet
{
    return NO;
}

- (void)drawRect:(NSRect)rect
{
    [super drawRect:rect];

    CGContextRef ctx = [[NSGraphicsContext currentContext] CGContext];
    if (!ctx) return;

    // Fill screen with black
    CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.0, 1.0);
    CGContextFillRect(ctx, NSRectToCGRect(self.bounds));

    pthread_mutex_lock(&_pixelLock);

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, _pixelBuffer, sizeof(_pixelBuffer), NULL);
    CGImageRef image = CGImageCreate(
        640, 480, 8, 32, 640 * 4, colorSpace,
        kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big,
        provider, NULL, NO, kCGRenderingIntentDefault
    );

    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    pthread_mutex_unlock(&_pixelLock);

    if (image) {
        // Calculate aspect-fit 4:3 rectangle inside bounds
        CGRect bounds = NSRectToCGRect(self.bounds);
        CGFloat scaleX = bounds.size.width / 640.0;
        CGFloat scaleY = bounds.size.height / 480.0;
        CGFloat scale = (scaleX < scaleY) ? scaleX : scaleY;

        CGFloat drawW = 640.0 * scale;
        CGFloat drawH = 480.0 * scale;
        CGFloat posX = bounds.origin.x + (bounds.size.width - drawW) / 2.0;
        CGFloat posY = bounds.origin.y + (bounds.size.height - drawH) / 2.0;

        CGRect targetRect = CGRectMake(posX, posY, drawW, drawH);
        CGContextDrawImage(ctx, targetRect, image);
        CGImageRelease(image);
    }
}

@end
