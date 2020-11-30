#pragma once
#include "Cannon.h"
#include "TargetsController.h"
using namespace std;
///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void ReadConfig();
	int ReadValue(IO::TextReader *reader, char *arg);
	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;
	void ReloadLevel();

	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;	

private:
	void Init();

private:
	float _timer;
	
	float _scale;
	float _angle;
	// Скорость движения снаряда пикселей / сек.
	float _speed;
	float _timeLimit;
	float _timeGame;
	
	Render::Texture* _tex1;
	Render::Texture* _tex2;
	Render::Texture* _tex3;
	Message mes;
	int _curTex;
	int _musik, _sound;
	// Количество мишеней
	int _count, _killVirus;
	bool _status, keyA;

	EffectsContainer _effCont;
	ParticleEffectPtr _eff;

	TimedSpline<FPoint> spline;
	Cannon *_cannon;
	TargetsController	*_targetsController;
};