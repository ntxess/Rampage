#pragma once

struct Health
{
	float maxHealth;
	float health;

	Health() 
		: maxHealth(100.f)
		, health(100.f) 
	{}
	
	Health(float val) 
		: maxHealth(val)
		, health(val)
	{}

	float operator+(const Health& other)
	{
		health += other.health;
		if (health > maxHealth) health = maxHealth;
		return health;
	}

	float operator-(const Health& other)
	{
		health -= other.health;
		if (health < 0) health = 0;
		return health;
	}

	float operator+(float val)
	{
		health += val;
		if (health > maxHealth) health = maxHealth;
		return health;
	}

	float operator-(float val) 
	{
		health -= val;
		if (health < 0) health = 0;
		return health;
	}

	Health& operator=(float val)
	{
		if (val > maxHealth) health = maxHealth;
		else if (val < 0) health = 0;
		else health = val;
		return *this;
	}

	inline void setMax(float val)
	{
		maxHealth = val;
		if (maxHealth < 0) maxHealth = 0;
		if (health > maxHealth) health = maxHealth;
	}

	inline bool isGood() const
	{
		return health > 0;
	}
};