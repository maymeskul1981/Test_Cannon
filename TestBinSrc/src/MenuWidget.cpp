#include "stdafx.h"
#include "MenuWidget.h"
#include "MyMessageFunc.h"

MenuWidget::MenuWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)	
	//, _targetsRuf(nullptr)	
	, _timer(0)	
{
	Init();
}

void MenuWidget::Init()
{	
	Core::guiManager.getLayer("TestLayer")->messageFunc = new MyMessageFunc();
	//_tex1 = Core::resourceManager.Get<Render::Texture>("Aim");
	//_tex2 = Core::resourceManager.Get<Render::Texture>("Circle");
	//_tex3 = Core::resourceManager.Get<Render::Texture>("Star");
/*
	_curTex = 0;
	_angle = 0;
	_speed = 20;
	//_count = 10;
	//_sample = MM::manager.PlaySample("track1");

	spline.addKey(0.0f, FPoint(100.0f, 100.0f));
	spline.addKey(0.25f, FPoint(150.0f, 300.0f));
	spline.addKey(0.5f, FPoint(500.0f, 300.0f));
	spline.addKey(0.75f, FPoint(630.0f, 450.0f));
	spline.addKey(1.0f, FPoint(600.0f, 550.0f));
	spline.CalculateGradient();
	*/
}

MenuWidget::~MenuWidget()
{
	
}

void MenuWidget::Draw()
{
	Render::BindFont("arial");	
	Render::PrintString(100, 360, "True", 1.f, CenterAlign);
	
	//
	// Получаем текущее положение курсора мыши.
	//
	IPoint mouse_pos = Core::mainInput.GetMousePos();

		
	//Render::device.MatrixTranslate(-texRect1.width * 0.5f, -texRect1.height * 0.5f, 0.0f);
	//Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);			
	Render::BindFont("arial");	
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, CenterAlign);
}

void MenuWidget::Update(float dt)
{	
	// Увеличиваем наш таймер с удвоенной скоростью.
	//
	_timer += dt * 2;
	
	//
	// Зацикливаем таймер в диапазоне (0, 2п).
	// Это нужно делать для предотвращения получения некорректных значений,
	// если вдруг переполнится разрядная сетка (float переполнился) или задержка
	// от предыдущего кадра была слишкой большой (система тормози-и-ит).
	//
	
	// Диапазон значений выбран равным (0, 2п), потому что мы используем это значение
	// для расчёта синуса, и большие значения будут просто периодически повторять результат.
	//
	while (_timer > 2 * math::PI)
	{
		_timer -= 2 * math::PI;
	}	
}

bool MenuWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// При нажатии на правую кнопку мыши, создаём эффект шлейфа за мышкой.
		//		
		
		//
		// И изменяем угол наклона текстуры.
		//
		
	}
	else
	{
		//
		// При нажатии на левую кнопку мыши, создаём временный эффект, который завершится сам.
		//
		
	}
	return false;
}

void MenuWidget::MouseMove(const IPoint &mouse_pos)
{	
}

void MenuWidget::MouseUp(const IPoint &mouse_pos)
{

}

void MenuWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//
	//Render::BindFont("arial");
	//Render::PrintString(100, 360, message.getData(), 1.f, CenterAlign);
	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void MenuWidget::KeyPressed(int keyCode)
{
	//
	// keyCode - виртуальный код клавиши.
	// В качестве значений для проверки нужно использовать константы VK_.
	//

	if (keyCode == VK_A) {
		// Реакция на нажатие кнопки A
		//Render::BindFont("arial");	
		//Render::PrintString(100, 360, "True", 1.f, CenterAlign);
		//Core::guiManager.getLayer("TestLayer")->messageFunc = new MessageFunc();
		Core::guiManager.getLayer("MenuLayer")->messageFunc = new MessageFunc();
	}
}

void MenuWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//

	if (unicodeChar == L'а') {
		// Реакция на ввод символа 'а'
	}
}
