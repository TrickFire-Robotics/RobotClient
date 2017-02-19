#ifndef PROJECTCONSTANTS_H_
#define PROJECTCONSTANTS_H_

namespace trickfire {

#ifndef GUI_ENABLED
#define GUI_ENABLED true
#endif

#ifndef SERVER_IP
#define SERVER_IP "127.0.0.1"
#endif

#ifndef OPENCV
#define OPENCV true
#endif

#ifndef CAMERA
#define CAMERA true
#endif

#ifndef KINECT
#define KINECT true
#endif

#ifndef CAMERA_SEND_FPS
#define CAMERA_SEND_FPS 24
#endif

#ifndef KINECT_SEND_FPS
#define KINECT_SEND_FPS 12
#endif

class ProjectConstants {
};
}

#endif
