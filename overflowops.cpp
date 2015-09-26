#include "Vertica.h"

using namespace Vertica;
using namespace std;

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
		if (c < a || c < b)
			vt_report_error(0, "Addition overflow.");
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
