#include "stdafx.h"
#include "TestWidget.h"


TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _curTex(0)
	, _targetsController(nullptr)
	, _cannon(nullptr)
	, _count(0)
	, _killVirus(0)
	, _timer(0)
	, _angle(0)
	, _eff(NULL)
	, _scale(0.f)
	, _musik(0)
	, _sound(0)
	, _status(false)
	, _speed(0)
	, _timeLimit(0)
	, _timeGame(0)
	, keyA(false)	
{
	Init();
}

void TestWidget::Init()
{
	ReadConfig();
	//Core::guiManager.getLayer("EndMenu")->messageFunc = new MyMessageFunc();
	_killVirus = 0;
	_cannon = new Cannon();
	_tex1 = Core::resourceManager.Get<Render::Texture>("Aim");
	_tex2 = Core::resourceManager.Get<Render::Texture>("Circle");

	_status = false;	
	_curTex = 0;
	_angle = 0;
	_timeGame = _timeLimit;
	_musik = MM::manager.PlaySample("track1");
	
	// Создаем новый контролер мишеней
	_targetsController = new TargetsController(_count, _timeLimit, _speed);
	spline.addKey(0.0f, FPoint(100.0f, 100.0f));
	spline.addKey(0.25f, FPoint(150.0f, 300.0f));
	spline.addKey(0.5f, FPoint(500.0f, 300.0f));
	spline.addKey(0.75f, FPoint(630.0f, 450.0f));
	spline.addKey(1.0f, FPoint(600.0f, 550.0f));
	spline.CalculateGradient();
}

void TestWidget::ReadConfig()
{
	// Открываем файл для чтения
	IO::FileStream fs("input.txt");
	IO::TextReader reader(&fs);
	
	// Считываем строку CountTarget - количество мишеней
	_count = ReadValue(&reader, "CountTarget");
	// Считываем строку SPEED - скорость движения снаряда (стрелы)
	_speed = ReadValue(&reader, "Speed");
	// Считываем строку TIME - лимит времени
	_timeLimit = ReadValue(&reader, "Time");
}

int TestWidget::ReadValue(IO::TextReader *reader, char *arg)
{
	std::string str, name, value;
	int result;

	str = reader->ReadAsciiLine();
	if (utils::ReadNvp(str, name, value) && name == arg)
	{
		result = utils::lexical_cast<int>(value);
	}
	else {
		result = 1;
	}
	return result;
}

void TestWidget::ReloadLevel()
{
	_targetsController->Clear();
	_status = false;
	_timeGame = _timeLimit;
	MM::manager.StopSample(_musik);
	_musik = MM::manager.PlaySample("track1");
	_targetsController->Init();
}

void TestWidget::Draw()
{	
	Render::BindFont("arial");
	//Render::GetColorStackSize();
	Render::SetColor(Color(200, 00, 00));
	if ((!_status)&&(_killVirus != _count))
	{		
		Render::PrintString(500, 730, utils::lexical_cast(_timeGame, 2), 2.f, LeftAlign);
	}
	else	
	{
		//delete _targetsController;
		if (_killVirus == _count)
		{
			Render::SetColor(Color(200, 00, 00));
			Render::PrintString(500, 730, "You are kill " + utils::lexical_cast((float)_count, 2) + " \n YOU WIN! \n Press key A for retry", 2.f, CenterAlign);
			Render::ResetColor();
		}
		else
		{
			Render::PrintString(500, 730, "You are kill " + utils::lexical_cast((float)_count, 2) + "\n Game Over \n Press key A for retry", 2.f, CenterAlign);
		}
		
	}
	Render::ResetColor();
	if (keyA)
	{
		//ReloadLevel();
		//Render::PrintString(100, 360, "True", 1.f, CenterAlign);
	}	
	
	//
	// Получаем текущее положение курсора мыши.
	//
	IPoint mouse_pos = Core::mainInput.GetMousePos();
	if (!_status) {
		_cannon->Draw();
		_targetsController->Draw();
	}

	//
	// Проталкиваем в стек текущее преобразование координат, чтобы в дальнейшем
	// можно было восстановить это преобразование вызовом PopMatrix.
	//
	Render::device.PushMatrix();
	
	//
	// Изменяем текущее преобразование координат, перемещая центр координат в позицию мыши
	// и поворачивая координаты относительно этого центра вокруг оси z на угол _angle.
	//
	Render::device.MatrixTranslate((float)mouse_pos.x, (float)mouse_pos.y, 0);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	if (!_curTex)
	{
		//
		// Метод Texture::Draw() выводит квадратный спрайт с размерами текстуры
		// в центре координат (0, 0). Центр координат и преобразование координат вершин
		// спрайта устанавливаются с применением текущего преобразования.
		//
		// При вызове метода Texture::Draw() вызывать Texture::Bind() необязательно.
		//
		IRect texRect1 = _tex1->getBitmapRect();
		FRect rect(texRect1);
		FRect uv(0, 1, 0, 1);
		_tex1->TranslateUV(rect, uv);
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-texRect1.width * 0.5f, -texRect1.height * 0.5f, 0.0f);
		//
		// Привязываем текстуру.
		//
		_tex1->Bind();

		//
		// Метод DrawQuad() выводит в графическое устройство квадратный спрайт, состоящий их двух
		// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
		// если разрешено текстурирование.
		//
		// Перед вызовом DrawQuad() должен быть вызов Texture::Bind() либо SetTexturing(false),
		// иначе визуальный результат будет непредсказуемым.
		//
		Render::DrawQuad(rect, uv);

		_tex1->Draw();
		Render::device.MatrixTranslate((float)mouse_pos.x - texRect1.width * 0.5f, (float)mouse_pos.y - texRect1.height * 0.5f, 0);
	}
	else
	{
		IRect texRect = _tex2->getBitmapRect();

		//
		// При отрисовке текстуры можно вручную задавать UV координаты той части текстуры,
		// которая будет натянута на вершины спрайта. UV координаты должны быть нормализованы,
		// т.е., вне зависимости от размера текстуры в текселях, размер любой текстуры
		// равен 1 (UV координаты задаются в диапазоне 0..1, хотя ничто не мешает задать их
		// больше единицы, при этом в случае установленной адресации текстуры REPEAT, текстура
		// будет размножена по этой стороне соответствующее количество раз).
		//

		FRect rect(texRect);
		FRect uv(0, 1, 0, 1);

		_tex2->TranslateUV(rect, uv);

		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-texRect.width * 0.5f, -texRect.height * 0.5f, 0.0f);

		//
		// Привязываем текстуру.
		//
		_tex2->Bind();
		
		//
		// Метод DrawQuad() выводит в графическое устройство квадратный спрайт, состоящий их двух
		// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
		// если разрешено текстурирование.
		//
		// Перед вызовом DrawQuad() должен быть вызов Texture::Bind() либо SetTexturing(false),
		// иначе визуальный результат будет непредсказуемым.
		//
		Render::DrawQuad(rect, uv);
	}

	//
	// Воостанавливаем прежнее преобразование координат, снимая со стека изменённый фрейм.
	//
	Render::device.PopMatrix();	
	
/*
	//
	// Этот вызов отключает текстурирование при отрисовке.
	//
	Render::device.SetTexturing(false);
	
	
	//
	// Метод BeginColor() проталкивает в стек текущий цвет вершин и устанавливает новый.
	//
	Render::BeginColor(Color(255, 128, 0, 255));	
	//
	// Метод DrawRect() выводит в графическое устройство квадратный спрайт, состоящий их двух
	// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
	// если разрешено текстурирование.
	//
	// Перед вызовом DrawRect() должен быть вызов Texture::Bind(), либо SetTexturing(false),
	// иначе визуальный результат будет непредсказуемым.
	//
	Render::DrawRect(924, 0, 100, 100);
	
	//
	// Метод EndColor() снимает со стека текущий цвет вершин, восстанавливая прежний.
	//
	Render::EndColor();
	
	//
	// Опять включаем текстурирование.
	//
	Render::device.SetTexturing(true);

	//
	// Рисуем все эффекты, которые добавили в контейнер (Update() для контейнера вызывать не нужно).
	//
	

	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, CenterAlign);
	*/
	_effCont.Draw();
}

void TestWidget::Update(float dt)
{
	if (!_status) 
	{
		_cannon->Update(dt);
		//
		// Обновим контейнер с эффектами
		//
		_effCont.Update(dt);
		_targetsController->Update(dt);
		if (_cannon->CheckTargetsHit(_targetsController, _effCont))
		{
			_sound = MM::manager.PlaySample("sound1");
		}
	}
	
	//
	// dt - значение времени в секундах, прошедшее от предыдущего кадра.
	// Оно может принимать разные значения, в зависимости от производительности системы
	// и сложности сцены.
	//
	// Для того, чтобы наша анимация зависела только от времени, и не зависела от
	// производительности системы, мы должны рассчитывать её от этого значения.
	//
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
	//
	// Анимирование параметра масштабирования в зависимости от таймера.
	//
	_scale = 0.8f + 0.25f * sinf(_timer);
	_timeGame -= dt;
	if (_timeGame < 0)
	{
		_status = true;
	}
	if (!_status) {
		_killVirus = _count - _targetsController->GetCountTargetNow();
	}
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// При нажатии на правую кнопку мыши, создаём эффект шлейфа за мышкой.
		//
		_eff = _effCont.AddEffect("Iskra");
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
		_eff->Reset();

		//
		// И изменяем угол наклона текстуры.
		//
		_angle += 25;
		while (_angle > 360)
		{
			_angle -= 360;
		}
	}
	else
	{
		//
		// При нажатии на левую кнопку мыши, создаём временный эффект, который завершится сам.
		//
		keyA = keyA ^ TRUE;
		ParticleEffectPtr eff = _effCont.AddEffect("FindItem2");
		eff->posX = mouse_pos.x + 0.f;
		eff->posY = mouse_pos.y + 0.f;
		eff->Reset();

		_cannon->MouseDown(mouse_pos, _speed, _effCont);
		//
		// Изменяем значение с 0 на 1 и наоборот.
		//
		//_curTex = 1 - _curTex;
	}
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
	if (_eff)
	{
		//
		// Если эффект создан, то перемещаем его в позицию мыши.
		//
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
	}
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	if (_eff)
	{
		//
		// Если эффект создан, то при отпускании мыши завершаем его.
		//
		_eff->Finish();
		_eff = NULL;
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//	
	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void TestWidget::KeyPressed(int keyCode)
{
	//
	// keyCode - виртуальный код клавиши.
	// В качестве значений для проверки нужно использовать константы VK_.
	//

	if (keyCode == VK_A) {
		// Реакция на нажатие кнопки A
		if ((_status) || (_killVirus == _count)) {
			ReloadLevel();
		}
		
		//keyA = keyA ^ TRUE;		
		//Core::guiManager.LoadLayers("TestLayer");
		//Core::mainScreen.popLayer();
		//Core::mainScreen.pushLayer("EndMenu");
	}
}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//

	if (unicodeChar == L'а') {		
		//keyA = keyA ^ TRUE;		
	}
}
