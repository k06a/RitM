#include "FileAdapter.h"

using namespace DiplomBukov;

FileAdapter::FileAdapter(char * filename, IRouter * router_)
	: router_(router_), file(NULL)
{
	file = fopen(filename, "rb");
	if (file == NULL)
		throw "Ошибка открытия файла";
}

FileAdapter::~FileAdapter()
{
	if (file != NULL)
		fclose(file);
}

void FileAdapter::setRouter(IRouter * router_)
{
	this->router_ = router_;
}

IRouter * FileAdapter::router()
{
	return router_;
}

void FileAdapter::run()
{
	while (true)
	{

	}
}