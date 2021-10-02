#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <stdlib.h>
#include <string.h>
// #include <EGL/egl.h>
// #include <GLES2/gl2.h>

#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO, "NativeActivity", __VA_ARGS__)



typedef struct app_state {
	// some app data
} app_state;



static void on_cmd_callback(struct android_app *app_glue, int32_t cmd) {
	app_state *state = (app_state *)app_glue->userData;
	
	switch (cmd) {
		case APP_CMD_SAVE_STATE:
			// Save state of app
			LOG_I("save state requested");
			app_glue->savedState = malloc(sizeof(app_state));
			if (app_glue->savedState) {
				*(app_state *)app_glue->savedState = *state;
				app_glue->savedStateSize = sizeof(app_state);
			}
			break;
		case APP_CMD_INIT_WINDOW:
			// create opengl context for example
			LOG_I("window got Initialized");
			break;
		case APP_CMD_TERM_WINDOW:
			// handle opengl context loss for example (stop drawing)
			LOG_I("window got terminated");
			break;
		default: break;
	}
}

static int32_t on_input_callback(struct android_app *app_glue, AInputEvent *event) {
	app_state *state = (app_state *)app_glue->userData;
	
	switch (AInputEvent_getType(event)) {
		case AINPUT_EVENT_TYPE_MOTION:
			if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE)
				LOG_I("MOVE EVENT pointercount: %zu", AMotionEvent_getPointerCount(event));
			break;
		default: break;
	}
	
	// return wether we consumed the event
	return 0;
}

void android_main(struct android_app* app_glue) {
	LOG_I("android_main started");
	
	app_state state = {};
	if (app_glue->savedState) {
		LOG_I("starting activity with saved state");
		state = *(app_state *)app_glue->savedState;
	}

	app_glue->userData = &state;
	app_glue->onAppCmd = on_cmd_callback;
	app_glue->onInputEvent = on_input_callback;
	
	while (1) {
		struct android_poll_source* source;
		while (ALooper_pollAll(0, nullptr, nullptr, (void **)&source) >= 0) {
			// let native-app-glue process the event.
			if (source)
				source->process(app_glue, source);

			// Check if we are exiting.
			if (app_glue->destroyRequested) {
				LOG_I("destroy requested");
				return;
			}
		}

		// do stuff
		// draw with opengl for example
	}
}
