#ifndef COMPONENT_VIEW_H
#define COMPONENT_VIEW_H

#include <Core.h>

class ComponentView
{
public:

	void OnGUIRender(Transform * component);

	void OnGUIRender(BezierPath* component);

	void OnGUIRender(const std::map<Texture*, std::string>& texture_cache, Renderable* renderable);
};



#endif