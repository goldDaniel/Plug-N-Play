#ifndef CURVE_EDITOR_H
#define CURVE_EDITOR_H

#include <Core.h>

/// <summary>
/// This class is responsible for manipulating a bezier curve 
/// </summary>
class CurveEditor
{
private:	

	/// <summary>
	/// The curve that is being edited 
	/// </summary>
	Bezier::Bezier<3> current_curve;

	/// <summary>
	/// The control point that is currently selected.
	/// This will be null if no selected point.
	/// </summary>
	Bezier::Point* held_point = nullptr;

	/// <summary>
	/// The position of the mouse.
	/// This is used to move points around & select them
	/// </summary>
	glm::vec2 mouse_world_pos = glm::vec2(0,0);

	/// <summary>
	/// The radius used for the control points.
	///
	/// Control points will be drawn at this size.
	///
	/// This is also the radius for the control points collision detection
	/// </summary>
	float selection_radius = 0.2f;

	/// <summary>
	/// If the mouse is held down.
	/// </summary>
	bool mouse_down = false;

public:
	
	/// <summary>
	/// Constructs our editor.
	/// Sets a default curve.
	/// </summary>
	CurveEditor()
	{
		current_curve = Bezier::Bezier<3>({ {-3, 6},{3, 0},{-3, 0},{-3, -6} });
	}

	/// <summary>
	/// This will check if the mouse is colliding with a control point.
	/// 
	/// Call this when a mouse button is pressed down.
	/// </summary>
	void OnMouseButtonDown() 
	{
		auto& control_points = current_curve.getControlPoints();
		for (auto& control_point : control_points)
		{
			glm::vec2 p{ control_point.x, control_point.y };
			if (glm::distance(mouse_world_pos, p) < selection_radius * 2)
			{
				held_point = &control_point;
			}
		}
	}

	/// <summary>
	/// This will release a held control point
	/// Call this when a mouse button is lifed.
	/// </summary>
	void OnMouseButtonUp()
	{
		//we want the point to be locked to the grid
		//so we round the position once the mouse is released
		if (held_point)
		{
			held_point->x = glm::round(held_point->x);
			held_point->y = glm::round(held_point->y);
		}
		held_point = nullptr;
		mouse_down = false;
	}

	/// <summary>
	/// Call this every frame. 
	/// If we are holding a point, this will update the position
	/// </summary>
	/// <param name="mouse_world_pos">
	/// The mouse point projected onto the xy0-Plane.
	/// The matrices used to unproject should be the ones 
	/// setting the shaperenderer view & proj
	/// </param>
	void Update(glm::vec2 mouse_world_pos)
	{
		this->mouse_world_pos = mouse_world_pos;
		if (held_point)
		{
			held_point->x = mouse_world_pos.x;
			held_point->y = mouse_world_pos.y;
		}
	}

	/// <summary>
	/// Renders the Curve and control points
	/// </summary>
	/// <param name="sh">
	/// The shaperenderer that will do the rendering
	/// </param>
	void DrawCurve(ShapeRenderer& sh)
	{
		const auto& control_points = current_curve.getControlPoints();
		sh.SetColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
		float step = 0.05f;
		for (float t = 0.f; t < 1; t += step)
		{
			Bezier::Point p00 = current_curve.valueAt(t);
			glm::vec2 p01(p00.x, p00.y);

			Bezier::Point p10 = current_curve.valueAt(t + step);
			glm::vec2 p11(p10.x, p10.y);

			sh.Line(p01, p11);
		}

		sh.SetColor(glm::vec4(0.8f, 0.8f, 0.3f, 1.f));
		for (std::size_t i = 1; i < control_points.size(); i++)
		{
			glm::vec2 p0(control_points[i - 1].x, control_points[i - 1].y);
			glm::vec2 p1(control_points[i].x, control_points[i].y);
			sh.Line(p0, p1);
		}

		for (const auto& point : control_points)
		{
			glm::vec2 p{ point.x, point.y };

			sh.SetColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
			if (glm::distance(mouse_world_pos, p) < selection_radius)
			{
				sh.SetColor(glm::vec4(1.f, 0.4f, 0.4f, 1.f));
			}
			sh.Circle(p, selection_radius, 16);
		}
	}

	/// <summary>
	/// Gets the control points for the bezier curve.
	/// Returns a copy, we dont want to expose the curve externally
	/// </summary>
	/// <returns>
	/// A copy of the control points for the bezier curve.
	/// </returns>
	const std::array<glm::vec2, 4> GetControlPoints()
	{
		std::array<glm::vec2, 4> result{};

		const auto& points = current_curve.getControlPoints();
		for (std::size_t i = 0; i < 4; i++)
		{
			result[i] = { points[i].x, points[i].y };
		} 

		return result;
	}

	/// <summary>
	/// Sets the curve we are working on.
	/// </summary>
	/// <param name="curve">
	/// The bezier curve that we want to edit
	/// </param>
	void SetCurve(const Bezier::Bezier<3>& curve)
	{
		this->current_curve = curve; 
	}

	/// <summary>
	/// Sets the curve back to a default
	/// </summary>
	void ResetCurve()
	{
		current_curve = Bezier::Bezier<3>({ {-3, 6},{3, 0},{-3, 0},{-3, -6} });
	}
};

#endif