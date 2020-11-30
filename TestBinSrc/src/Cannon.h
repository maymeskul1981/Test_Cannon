#pragma once
#include "TargetsController.h"
#include <vector>

class Bullet
{
public:
	// Конструктор с параметрами - коеффициент величины текстуры, 
	// скорость полета снаряда и угол наклона текстуры
	Bullet(float scale, float speed, float angle, EffectsContainer &effCont);
	// Диструктор
	~Bullet();

	void Draw();
	void Update(float dt);

	// Установить значение флага "попадание"
	void setHit(bool hit);
	// получить значение флага "попадание"
	bool getHit();
	// получить позицию снаряда
	FPoint getPosition();
	// Убиваем эффект
	void KillEffect();

private:
	void Init();

private:
	// Текстура стрелы
	Render::Texture *_bullet;
	// Позиция
	FPoint _position;
	// Угол поворота текстуры
	float _angle;
	// Масштаб текстуры
	float _scale;
	// Скорость снаряда (пикс / сек)
	float _speed;
	// Флаг "попадание"
	bool _isHit;
	// Коэффициент времени
	int _kTime;

	ParticleEffectPtr _eff;
	EffectsContainer _effCont;
};

// -------------------------- Gun Class -----------------------------
// Класс "Оружие". Пушка внизу экрана
class Cannon
{
public:
	Cannon();
	void Draw();
	void Update(float dt);
	// Нажатие кнопки мыши
	bool MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont);
	// Получить количество выпущенных стрел
	//int getCountBullet();
	// Удалить все стрелы
	void ClearBullets();
	// Проверка попадания в мишени
	bool CheckTargetsHit(TargetsController *targetsRuf, EffectsContainer &effCont);
	// Удаляем пулю из вектора
	void DeletBullet(unsigned n);
	int Cannon::getCountBullet();

private:
	void Init();

private:
	// Текстура пушки
	Render::Texture *_cannon;
	// Положение пушки
	IPoint _posCannon;
	// Масштаб текстуры
	float _scale;
	// Угол поворота текстуры
	float _angle;
	// Вектор с пулями
	std::vector<Bullet *> _bullets;
};