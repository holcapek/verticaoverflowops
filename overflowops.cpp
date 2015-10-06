#include "Vertica.h"
#include "BuildInfo.h"

using namespace Vertica;
using namespace std;

// https://github.com/postgres/postgres/blob/aa9eac45ea868e6ddabc4eb076d18be10ce84c6a/src/backend/utils/adt/int8.c#L28
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
		if ( ( SAMESIGN(a, b) && !SAMESIGN(c, a) ) || ( c == vint_null ) )
			vt_report_error(0, "int out of range");

		res_writer.setInt(c);
		res_writer.next();
	}
	while (arg_reader.next());
}

}; // class OverflowAdd

class OverflowSub : public ScalarFunction
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
		const vint c = a - b;

		// https://github.com/postgres/postgres/blob/aa9eac45ea868e6ddabc4eb076d18be10ce84c6a/src/backend/utils/adt/int8.c#L534
		if ( ( !SAMESIGN(a, b) && !SAMESIGN(c, a) ) || ( c == vint_null ) )
			vt_report_error(0, "int out of range");

		res_writer.setInt(c);
		res_writer.next();
	}
	while (arg_reader.next());
}

}; // class OverflowSub

class OverflowAddFactory : public ScalarFunctionFactory
{
public:

virtual ScalarFunction*
createScalarFunction(ServerInterface &interface)
{
	return vt_createFuncObj(interface.allocator, OverflowAdd);
}

virtual void
getPrototype(
	ServerInterface &interface,
	ColumnTypes &argTypes,
	ColumnTypes &returnType
) {
	argTypes.addInt();
	argTypes.addInt();
	returnType.addInt();
}

}; // class OverflowAddFactory

class OverflowSubFactory : public ScalarFunctionFactory
{
public:

virtual ScalarFunction*
createScalarFunction(ServerInterface &interface)
{
	return vt_createFuncObj(interface.allocator, OverflowSub);
}

virtual void
getPrototype(
	ServerInterface &interface,
	ColumnTypes &argTypes,
	ColumnTypes &returnType
) {
	argTypes.addInt();
	argTypes.addInt();
	returnType.addInt();
}

}; // class OverflowSubFactory

RegisterFactory(OverflowAddFactory);
RegisterFactory(OverflowSubFactory);

RegisterLibrary(
	"Jan Holcapek <holcapek@gmail.com>",
	"TODO library_build_tag",
	"TODO library_version",
	VERTICA_BUILD_ID_Brand_Version,
	"https://github.com/holcapek/verticaoverflowops",
	"User defined scalar functions providing overflow-aware arithmetic operators",
	"TODO licenses_required",
	"TODO signature"
);
