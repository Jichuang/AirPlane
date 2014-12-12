#include "AppDelegate.h"
#include "WelcomeScene.h"
#include <vector>

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLView::createWithRect("AirPlane",Rect(0,0,320.0f,480.0f));
		//glview->setFrameSize(480,760);
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(480.0f,760.0f,ResolutionPolicy::SHOW_ALL);//竖屏
	//std::vector<std::string> searchPath; //若 上面改为 （320.0f,480.0f）
	//searchPath.push_back("w640");
	//CCFileUtils::getInstance()->setSearchPaths(searchPath);
	//director->setContentScaleFactor(840.0f/560.0f);
	//glview->setDesignResolutionSize(480,800,kResolutionShowAll);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = WelcomeScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}


//资源路径 (cocos2d-x 根据相对路径加载资源)
void AppDelegate::setResourceSearchResolution()
{
	//不用也行
	std::vector<std::string> paths;
	paths.push_back("font");
	paths.push_back("ui");
	paths.push_back("sound");
	FileUtils::sharedFileUtils()->setSearchPaths(paths);
}