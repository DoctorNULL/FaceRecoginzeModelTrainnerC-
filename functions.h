void createDataset(string name , VideoCapture cam, CascadeClassifier det)
{
	string cmd = "mkdir " + name;
	stringstream path;
	int c = 0;
	Mat frame;
	std::vector<Rect> face;

	system(cmd.c_str());

	while (c < dataLen)
	{
		cam.read(frame);

		det.detectMultiScale(frame, face);

		if (!face.empty())
			cv::rectangle(frame, face[0], Scalar(0, 0, 255), 2);

		imshow("Cam", frame);
		if (waitKey(1) >= 0)
		{
			path << name << "\\" << c << ".jpg";

			frame = frame(face[0]);

			imwrite(path.str(), frame);
			cout << "Saved in : " << path.str() << endl;
			path.str("");
			c++;
		}
	}

	destroyWindow("Cam");
}

person createPerson(VideoCapture cam,CascadeClassifier det)
{
	string name;
	int id;
	person p;
	ofstream file("Data.data",ios::app);

	cout << "Enter Label : ";
	cin >> id;

	cout << "Enter Name : ";
	cin >> name;

	p.set(id,name);

	createDataset(name, cam, det);

	file << p;

	cout << "Person Created Successfuly...\n";

	file.close();
	return p;
}

operation getOperation()
{
	int x;

	cout << "1:Add Person\n";
	cout << "2:Train Model\n";
	cout << "3:Recognize\n";
	cout << "4:Exit...\n";

	cin >> x;

	while (x < 1 || x > 4)
	{
		cout << "Wrong Value...\n";
		cin >> x;
	}

	switch (x)
	{
	case 1:
		return createData;
		break;
	case 2 : 
		return Train;
		break;
	case 3 :
		return Look;
		break;
	case 4 :
		return Exit;
		break;
	}
}

bool getAllPersons(vector<person>& persons)
{
	person p;
	ifstream file;

	file.open("Data.data",ios::in);

	if (!file.is_open())
	{
		cout << "Can't Open Data File...\n";
		return false;
	}

	while (!file.fail())
	{
		file >> p;
		persons.push_back(p);
	}

	file.close();
	return true;
}

bool prepareForTrainning (vector<Mat>&imgs,vector<int>&labels)
{
	vector<person> p;
	Mat img;
	stringstream s;

	if (!getAllPersons(p))
	{
		cout << "Can't Get All Persons \n";
		return false;
	}

	for (int i = 0; i < p.size(); i++)
	{
		for (int j = 0; j < dataLen; j++)
		{
			s << p[i].getName() << "\\" << j << ".jpg";
			img = imread(s.str());
			s.str("");

			resize(img,img,Size(imgSize,imgSize));
			cvtColor(img, img, COLOR_BGR2GRAY);

			imgs.push_back(img);
			labels.push_back(p[i].getID());
		}
	}

	return true;
}

bool TrainModel()
{
	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();

	vector<Mat> imgs;
	vector<int> labels;

	time_t start, end;

	double waitingTime;

	if (!prepareForTrainning(imgs, labels))
	{
		cout << "Error While Preparing \n";
		return false;
	}

	try
	{
		time(&start);

		model->train(imgs, labels);

		time(&end);

		waitingTime = difftime(start,end);

		cout << "Train done in " << waitingTime << " seconds\n";
	}
	catch (Exception& e)
	{
		cout << "Can't Train Model...\n";

		cerr << "Massage " << e.what() << endl;

		return false;
	}
	

	model->write("Model.data");

	cout << "Model Trained Succussfully...\n";

	return true;
}

person getPersonById(vector<person> persons,int id)
{
	for (int i = 0; i < persons.size(); i++)
	{
		if (persons[i].getID() == id)
			return persons[i];
	}

	return person();
}

void recoginze(VideoCapture cam, CascadeClassifier det)
{
	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();

	try
	{
		model->read("Model.data");
	}
	catch (Exception& e)
	{
		cout << "Can't Load Model ...\n";
		cerr << "Error in " << e.what() << endl;

		return;
	}

	vector<person> p;

	if (!getAllPersons(p))
	{
		cout << "Can't Open Data File ...\n";
		return;
	}

	Mat frame, temp;
	vector<Rect> faces;
	int predictedLabel;
	double c, persentage;
	stringstream s;
	person x;

	while (waitKey(1) < 0)
	{
		cam.read(frame);

		det.detectMultiScale(frame,faces);

		for (int i = 0; i < faces.size(); i++)
		{
			temp = frame(faces[i]);
			cvtColor(temp, temp, COLOR_BGR2GRAY);
			resize(temp, temp, Size(imgSize, imgSize));

			model->predict(temp, predictedLabel, c);

			persentage = 100 - c;

			if (persentage >= 65)
			{
				rectangle(frame, faces[i], Scalar(0, 255, 0));
				x = getPersonById(p, predictedLabel);
				s << x.getName() << " : " << persentage << "%";
				putText(frame, s.str(), Point(faces[i].x + 20, faces[i].y + 50), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0));
				s.str("");
			}
			else
			{
				rectangle(frame, faces[i], Scalar(0, 0, 255));
				putText(frame, "Unknown", Point(faces[i].x + 20, faces[i].y + 50), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
			}
		}
		imshow("Cam",frame);
	}

	destroyWindow("Cam");
}