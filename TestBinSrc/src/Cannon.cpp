#include "stdafx.h"
#include "Cannon.h"
#include "TestWidget.h"
#include "TargetsController.h"

Bullet::Bullet(float scale, float speed, float angle, EffectsContainer &effCont) {
	// Эффект
	_eff = effCont.AddEffect("FireWorks");
	// Установка значений свойств из параметров конструктора
	_scale = scale;
	_speed = speed;
	_angle = angle;
	// Инициализация
	Init();
}

void Bullet::Init() {	
	_kTime = 10;
	// Загрузка текстуры стрелы
	_bullet = Core::resourceManager.Get<Render::Texture>("Bullet");
	// Установка начальной позиции
	_position = IPoint(TestWidget::WINDOW_WIDTH / 2, 10);
	_isHit = false;

	// Добавление эффекта горения наконечника и шлейфа из дыма
	_eff->posX = _position.x;
	_eff->posY = _position.y;
	_eff->Reset();
}

Bullet::~Bullet() {

}

FPoint Bullet::getPosition() {
	return _position;
}

// Устанновка флага "попадание"
void Bullet::setHit(bool hit) {
	_isHit = hit;
	// Завершаем эффект с сохранением шлейфа
	if (_eff) {
	_eff->Finish();
	_eff = NULL;
	}
}

void Bullet::Draw() {
	// Обновляем контейнер эффектов
	_effCont.Draw();
	
	// Если еще попадания не было	
	if (!_isHit) {
		Render::device.PushMatrix();
		// Сдвигаем на текущую позицию
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		// Поворачиваем на нужный угол
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		// Получаем ширину текстуры
		IRect texRect = _bullet->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_bullet->TranslateUV(rect, uv);

		// Устанавливаем масштаб текстуры
		Render::device.MatrixScale(_scale);
		// Сдвигаем на "наконечник" стрелы
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height, 0.f));

		// Натягиваем текстуру
		_bullet->Bind();
		Render::DrawQuad(rect, uv);
		Render::device.PopMatrix();
	}
}

void Bullet::Update(float dt) {
	// Подсчитываем положение стрелы
	_position.x += _speed * math::cos((_angle + 90) * math::PI / 180) * dt * _kTime;
	_position.y += _speed * math::sin((_angle + 90) * math::PI / 180) * dt * _kTime;

	_effCont.Update(dt);
	// Если попадания не было
	if (!_isHit) {
		// Сдвигаем эффект
		_eff->posX = _position.x;
		_eff->posY = _position.y;
	}
}

bool Bullet::getHit() {
	return _isHit;
}

void Bullet::KillEffect() {
	if (_eff) {
		_eff->Kill();
		_eff = NULL;
	}
}

// --------------------------- Gun Class -------------------
// Класс пушка

Cannon::Cannon() {
	Init();
}

void Cannon::Init() {
	// Загружаем текстуру пушки
	_cannon = Core::resourceManager.Get<Render::Texture>("Cannon");
	// Масштаб подоран оптимальный для размера текстуры пушки и снарядов
	_scale = 0.2;
	// Начальный угол 0 градусов
	_angle = 0;
	// Позиция внизу по середине экрана
	_posCannon = IPoint(TestWidget::WINDOW_WIDTH / 2, 50);
}

void Cannon::Draw() {
	// Рисуем стрелы, если они есть
	for (unsigned i = 0; i < _bullets.size(); i++) {
		_bullets[i]->Draw();
	}

	// Рисуем пушку с наклоном относительно положения мышки
	Render::device.PushMatrix();

	Render::device.MatrixTranslate(math::Vector3(_posCannon.x, _posCannon.y, 0));
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	IRect texRect = _cannon->getBitmapRect();
	FRect rect(texRect), uv(0, 1, 0, 1);
	_cannon->TranslateUV(rect, uv);

	Render::device.MatrixScale(_scale);
	Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2.f, 0.f));

	_cannon->Bind();

	Render::DrawQuad(rect, uv);

	Render::device.PopMatrix();
}

void Cannon::DeletBullet(unsigned n) {
	// То удаляем пулю из памяти
	_bullets[n]->KillEffect();
	delete _bullets[n];
	// и из вектора
	_bullets.erase(_bullets.begin() + n);
}

void Cannon::Update(float dt) {
	unsigned i = 0;
	IRect texRect = _cannon->getBitmapRect();
	// Обновляем все стрелы которые есть	
	while (i < _bullets.size()) {
		_bullets[i]->Update(dt);
		// Проверяем, если стрела вышла за пределы экрана
		FPoint aPoint = _bullets[i]->getPosition();		
		if ((aPoint.y > (TestWidget::WINDOW_HEIGHT + texRect.height)) ||
			(aPoint.y < (-texRect.height)) ||
			(aPoint.x > (TestWidget::WINDOW_WIDTH + texRect.height)) ||
			(aPoint.x < (-texRect.height))) {
			// То удаляем стрелу из памяти
			DeletBullet(i);
		}
		else {
			i++;
		}
	}
	IPoint mouse_pos = Core::mainInput.GetMousePos();
	// Высчитываем угол наклона пушки и снарядов
	if ((mouse_pos.x - _posCannon.x) != 0) {
		_angle = math::atan((mouse_pos.y - _posCannon.y), (mouse_pos.x - _posCannon.x)) * 180.0f / math::PI - 90;
	}
}

// Нажата левая кнопка мышки
bool Cannon::MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont) {	
	// Создаем новую пулю
	_bullets.push_back(new Bullet (_scale, speed, _angle, effCont));
	return false;
}

// Проверка попадания в мишень
bool Cannon::CheckTargetsHit(TargetsController *targetsRuf, EffectsContainer &effCont) {
	
	bool status = false;
	for (int i = _bullets.size() - 1; i >= 0; i--) {
		// Если стрела еще не попадала ни в одну мишень
		if (!_bullets[i]->getHit()) {
			// И если она попала в мишень
			if (targetsRuf->ChekingHits(_bullets[i]->getPosition(), effCont)) {
				// То устанавливаем флаг "попадание"
				_bullets[i]->setHit(true);
				status = true;
			}
		}
	}	
	return status;
}


// Удаляем все стрелы
void Cannon::ClearBullets() {
	
	for (int i = _bullets.size() - 1; i >= 0; i--) {
		_bullets[i]->KillEffect();
		delete _bullets[i];
	}
	_bullets.clear();	
}


int Cannon::getCountBullet() {
	return _bullets.size();
}