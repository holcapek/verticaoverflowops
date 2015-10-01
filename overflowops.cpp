#include "Vertica.h"

using namespace Vertica;
using namespace std;

#define SAMESIGN(a,b) (((a)<0)==((b)<0))

class OverflowAdd : public ScalarFunction
{

public:
virtual void
processBlock(
	ServerInterface &srvInterface,
	BlockReader     &arg_reader,
	BlockWriter     &res_writer
) {

	do
	{
		const vint a = arg_reader.getIntRef(0);
		const vint b = arg_reader.getIntRef(1);
		const vint c = a + b;

		// https://github.com/postgres/postgres/blob/aa9eac45ea868e6ddabc4eb076d18be10ce84c6a/src/backend/utils/adt/int8.c#L513

		// TODO handle c == vint_null
		if (SAMESIGN(a, b) && !SAMESIGN(c, a))
			vt_report_error(0, "vint out of range");

		res_writer.setInt(c);
		res_writer.next();
	}
	while (arg_reader.next());
}

};

class OverflowAddFactory : public ScalarFunctionFactory
{
public:

virtual ScalarFunction*
createScalarFunction(ServerInterface &interface)
{
	return vt_createFuncObj(interface.allocator, OverflowAdd);
}

virtual void getPrototype(ServerInterface &interface,
	ColumnTypes &argTypes,
	ColumnTypes &returnType)
	{
		argTypes.addInt();
		argTypes.addInt();
		returnType.addInt();
	}
};

RegisterFactory(OverflowAddFactory);
