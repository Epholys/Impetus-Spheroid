#include "gui/Component.hpp"


namespace gui
{

//-----------------------------------------------------------------------------
// *** Construction and Destruction: ***

Component::Component()
	: isSelected_(false)
	, isActive_(false)
{
}

Component::~Component()
{
}

//-----------------------------------------------------------------------------
// *** Selection: ***

bool Component::isSelected() const
{
	return isSelected_;
}

void Component::select()
{
	isSelected_ = true; 
}

void Component::deselect()
{
	isSelected_ = false;
}

//-----------------------------------------------------------------------------
// *** Activation: ***

bool Component::isActive() const
{
	return isActive_;
}

void Component::activate()
{
	isActive_ = true;
}

void Component::deactivate()
{
	isActive_ = false;
}

}
