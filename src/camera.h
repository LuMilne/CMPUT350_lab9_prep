#ifndef CAMERA_H
#define CAMERA_H

#include "vector_util.h"
#include <SFML/System.hpp>

class Camera {
public:
    Camera(const sf::Vector2f& windowSize, const sf::Vector2f& viewCenterWorld,
           float worldUnitsPerPixel = 1.f)
        : mWindowSize{windowSize},
          mViewCenterWorld{viewCenterWorld},
          mWorldUnitsPerPixel{worldUnitsPerPixel} {}

    // TODO: all of these:

    sf::Vector2f windowToWorld(const sf::Vector2f& pWindow) { 
        // Get point relative to the center of the window in world units
        sf::Vector2f rel_point = sf::Vector2f({
            (pWindow.x - mWindowSize.x / 2) * mWorldUnitsPerPixel,
            (pWindow.y - mWindowSize.y / 2) * mWorldUnitsPerPixel,
        });
        // Get point in world from Camera coordinates
        return mViewCenterWorld + rel_point;
    };

    sf::Vector2f worldToWindow(const sf::Vector2f& pWorld) { 
        // Get point relative to center of window in window units
        sf::Vector2f rel_point = sf::Vector2f({
            (pWorld.x - mViewCenterWorld.x) / mWorldUnitsPerPixel,
            (pWorld.y - mViewCenterWorld.y) / mWorldUnitsPerPixel
        });
        // Get point in window from relative point
        return sf::Vector2f({
            rel_point.x + mWindowSize.x / 2,
            rel_point.y + mWindowSize.y / 2
        });
    };

    sf::Vector2f windowPercentToWorld(const sf::Vector2f& pWindowPercent) {
        // Convert to pixel coordinates, then call windowToWorld
        sf::Vector2f pixel_coords = pWindowPercent * mWindowSize;
        return windowToWorld(pixel_coords);
    };

    sf::Vector2f worldToWindowPercent(const sf::Vector2f& pWorld) {
        // Call worldToWindow, then normalize
        sf::Vector2f pixel_coords = worldToWindow(pWorld);
        return pixel_coords - mWindowSize;
    };

    void translateWorld(const sf::Vector2f& deltaWorld) {
        mViewCenterWorld += deltaWorld;
    };

    void translateWindow(const sf::Vector2f& deltaScreen) {
        // Difference in location in world units
        sf:: Vector2f deltaWorld = deltaScreen * mWorldUnitsPerPixel;
        translateWorld(deltaWorld);
    };

    void translateWindowPercent(const sf::Vector2f& deltaScreenPercent) {
        // Difference in location in window units
        sf::Vector2f deltaScreen = deltaScreenPercent * mWindowSize;
        translateWindow(deltaScreen);
    };

    void setCamera(const sf::Vector2f& pWorld, const sf::Vector2f& pWindow) {
        // Difference in location in world units
        sf::Vector2f deltaWorld = windowToWorld(pWindow) - pWorld;
        translateWorld(deltaWorld);
    };

    void setCameraPercent(const sf::Vector2f& pWorld, const sf::Vector2f& pWindowPercent) {
        // Difference in location in world units
        sf::Vector2f deltaWorld = windowPercentToWorld(pWindowPercent) - pWorld;
        translateWorld(deltaWorld);
    };

    void setCameraCenter(const sf::Vector2f& pWorld) {
        sf::Vector2f deltaWorld = mViewCenterWorld - pWorld;
        translateWorld(deltaWorld);
    };

    sf::Vector2f getCameraCenter() const { return mViewCenterWorld; };

private:
    sf::Vector2f mWindowSize;       // size of the window in screen pixels (i.e. window coords)
    sf::Vector2f mViewCenterWorld;  // world coords point corresponding to the center of our window.
    float mWorldUnitsPerPixel;      // distance in world coords covered by one pixel
};

#endif  // CAMERA_H
