#pragma once
///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
#include "MyMessageFunc.h"

class MenuWidget : public GUI::Widget
{
public:
	MenuWidget(const std::string& name, rapidxml::xml_node<>* elem);
	~MenuWidget();
	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;
	
	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;	

private:
	void Init();

private:
	float _timer;
	//float _scale;
	//float _angle;
	// Скорость движения снаряда пикселей / сек.
	//float _speed;
	
	//Render::Texture* _tex1;
	//Render::Texture* _tex2;
	//Render::Texture* _tex3;
	//int _curTex;
	//int _sample;
	//int	_count;

	//EffectsContainer _effCont;
	//ParticleEffectPtr _eff;
	//TargetsController *_targetsRuf;
	//TimedSpline<FPoint> spline;
	
};
