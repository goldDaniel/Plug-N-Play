#ifndef CURVE_EDITOR_H
#define CURVE_EDITOR_H

#include <Core.h>

class CurveEditor
{
private:	
	Bezier::Bezier<3> current_curve;
	Bezier::Point* held_point = nullptr;

	glm::vec2 mouse_world_pos;

	float selection_radius = 0.2f;
	bool mouse_down = false;

public:
	
	CurveEditor()
	{
		current_curve = Bezier::Bezier<3>({ {-3, 6},{3, 0},{-3, 0},{-3, -6} });
	}


	void OnMouseButtonDown()
	{
		mouse_down = true;
	}

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

	void Update(glm::vec2 mouse_world_pos)
	{
		this->mouse_world_pos = mouse_world_pos;

		auto& control_points = current_curve.getControlPoints();
		if (held_point)
		{
			held_point->x = mouse_world_pos.x;
			held_point->y = mouse_world_pos.y;
		}
		else
		{
			if (mouse_down)
			{
				for (auto& control_point : control_points)
				{
					glm::vec2 p{ control_point.x, control_point.y };
					if (glm::distance(mouse_world_pos, p) < selection_radius * 2)
					{
						held_point = &control_point;
					}
				}
			}
		}
	}

	void DrawCurve(ShapeRenderer& sh)
	{
		auto& control_points = current_curve.getControlPoints();
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

		sh.SetColor(glm::vec4(0.3f, 0.3f, 0.8f, 1.f));
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

	
	

	void ResetCurve()
	{
		current_curve = Bezier::Bezier<3>({ {-3, 6},{3, 0},{-3, 0},{-3, -6} });
	}
};

#endif