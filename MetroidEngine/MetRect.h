#pragma once

struct MetRect
{
	MetRect(){};
	MetRect(float X, float Y, float W, float H)
	{
		this->X = X;
		this->Y = Y;
		this->W = W;
		this->H = H;
	}
	float X, Y, W, H;
};