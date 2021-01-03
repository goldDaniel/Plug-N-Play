#ifndef COMPONENT_VIEW_H
#define COMPONENT_VIEW_H


class ComponentView
{
public:

	template<class T>
	void OnGUIRender(T* component);
};



#endif