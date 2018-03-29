#include "SubjectObserver.h"

int main()
{
	Subject<int> sub01("SUB-01"); Subject<int> sub02("SUB-02");
	Observer<int> obs01("OBS-01"); Observer<int> obs02("OBS-02");

	sub01.Attach(&obs01, "obs01 attached to sub01");
	sub01.Attach(&obs02, "obs02 attached to sub01");
	sub02.Attach(&obs01, "obj01 attached to sub02");

	sub01.Notify(11);
	sub02.Notify(22);
	sub01.Notify(33);

	return 0;
}

