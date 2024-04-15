#pragma once

class nonmoveable
{
public:
	nonmoveable(nonmoveable&&) = delete;     // 
	nonmoveable& operator=(nonmoveable&&) = delete;  // 

protected:
	nonmoveable() = default;  // 
	~nonmoveable() = default;  // 
};