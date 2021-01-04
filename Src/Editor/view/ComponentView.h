#ifndef COMPONENT_VIEW_H
#define COMPONENT_VIEW_H

#include <Core.h>

class ComponentView
{
public:

	void OnGUIRender(Transform * component);

	void OnGUIRender(const std::unordered_map<std::string, Bezier::Bezier<3>>& path_cache, BezierPath* component);

	void OnGUIRender(const std::unordered_map<std::string, Texture*>& texture_cache, Renderable* renderable);
};



#endif