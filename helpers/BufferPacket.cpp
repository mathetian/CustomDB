#include "BufferPacket.h"

BufferPacket::BufferPacket(int size)
{
	this -> size = size;
	this -> data = new char[size];
	this -> cur  = 0;
	log = Log::GetInstance();
}

BufferPacket::~BufferPacket()
{
	if(data) delete [] data;
	data = NULL;
}

BufferPacket & BufferPacket::operator << (int ivalue)
{
	if(cur + sizeof(int) > size)
		log -> _Error("BufferPacket error << int\n");
	else
	{
		*(int*)(data + cur) = ivalue;
		cur += sizeof(int);
	}
	return *this;
}

BufferPacket & BufferPacket::operator << (size_t st)
{
	if(cur + sizeof(size_t) > size)
		log -> _Error("BufferPacket error << size_t\n");
	else
	{
		*(size_t*)(data + cur) = st;
		cur += sizeof(size_t);
	}
	return *this;
}

BufferPacket & BufferPacket::operator << (string str)
{
	if(cur + str.size() > size)
		log -> _Error("BufferPacket error << string\n");
	else
	{
		memcpy(data + cur, str.c_str(), str.size());
		cur += str.size();
	}
	return *this;
}

void BufferPacket::write(char * str, int len)
{
	if(cur + len > size) 
		log -> _Error("BufferPacket error write\n");
	else
	{
		memcpy(data + cur, str, len);
		cur += len;
	}
}

BufferPacket & BufferPacket::operator >> (int&ivalue)
{
	if(cur + sizeof(ivalue) > size) 
		log -> _Error("BufferPacket >> int overflow\n");
	else
	{
		ivalue = *(int*)(data + cur);
		cur += sizeof(ivalue);
	}
	return *this;
}

BufferPacket & BufferPacket::operator >> (size_t&st)
{
	if(cur + sizeof(st) > size)
		log -> _Error("BufferPacket >> size_t overflow\n");
	else
	{
		st = *(size_t*)(st + cur);
		cur += sizeof(st);
	}
	return *this;
}

BufferPacket & BufferPacket::operator >> (string&str)
{
	if(cur + str.size() > size)
		log -> _Error("BufferPacket >> string overflow\n");
	else
	{
		int index = 0;
		for(;index < str.size();index++)
			str[index] = data[cur + index];
		cur += str.size();
	}
	return *this;
}

void BufferPacket::read(char * str, int len)
{
	if(cur + len > size)
		log -> _Error("BufferPacket >> char* overflow\n");
	else
	{
		memcpy(str,data + cur,len);
		cur += len;
	}
}

void BufferPacket::write(BufferPacket packet)
{
	if(cur + packet.getSize() > size)
		log -> _Error("BufferPacket write packet overflow\n");
	else
	{
		memcpy(data + cur,(char*)&packet,packet.getSize());
		cur += packet.getSize();
	}
}