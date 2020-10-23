#include "types.h"
#include "functions.h"

int main()
{
	VideoCapture cam(0);
	CascadeClassifier det;

	if (!cam.isOpened())
	{
		cout << "Can't Open Cam\n";
		return EXIT_FAILURE;
	}

	if (!det.load("cascade.xml"))
	{
		cout << "Can't Load Detector\n";
		return EXIT_FAILURE;
	}

	operation op;

	while (true)
	{
		op = getOperation();

		switch (op)
		{
		case createData:
			createPerson(cam, det);
			break;
		case Train:
			if (!TrainModel())
				cout << "Can't Train Model...\n";
			break;
		case Look:
			recoginze(cam,det);
			break;
		case Exit:
			cam.release();
			return EXIT_SUCCESS;
			break;
		default:
			break;
		}
	}

	return EXIT_SUCCESS;
}