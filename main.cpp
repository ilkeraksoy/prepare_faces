#include "Include.h"

int main(int argc, char **argv) {

	string input_folder, output_folder;

	input_folder = argv[1];
	output_folder = argv[2];

	Folder inputFolder(input_folder);
	Folder outputFolder(output_folder);
	outputFolder.create();

	int input_numberOfSubfolders = inputFolder.getNumberOfSubfolders();
	int output_numberOfSubfolders = inputFolder.getNumberOfSubfolders();

	Folder *inputSubFolders = new Folder[input_numberOfSubfolders];
	Folder *outputSubFolders = new Folder[input_numberOfSubfolders];

	vector<string> input_subFolderPathNames;
	inputFolder.getSubfolderPathNames(input_subFolderPathNames);

	FaceDetector fd(FACE_CASCADE_PATH, LEFT_EYE_CASCADE_PATH, RIGHT_EYE_CASCADE_PATH, FACE_DETECT_SCALE_FACTOR, FACE_DETECT_MIN_NEIGHBORS, FACE_DETECT_MIN_SIZE, FACE_DETECT_MAX_SIZE);
	FaceAligner fa(FACE_ALIGN_DESIRED_LEFT_EYE_X, FACE_ALIGN_DESIRED_LEFT_EYE_Y, FACE_ALIGN_FACE_SIZE);

	std::ofstream list;
	std::ofstream names;

	for (int i = 0; i < input_numberOfSubfolders; i++) {

		inputSubFolders[i].setPath(input_subFolderPathNames[i]);
		outputSubFolders[i].setPath(output_folder + "/" + inputSubFolders[i].getName());
		outputSubFolders[i].create();

		names.open(output_folder + "/" + "names.txt", ios::app);
		names << outputSubFolders[i].getName() << endl;
		names.close();

		vector<string> input_filePathNames;
		inputSubFolders[i].getFilePathNames(input_filePathNames);

		vector<Rect> faces_r;		
		vector<Rect> leftEyes_r;
		vector<Rect> rightEyes_r;

		vector<string> output_file_list;

		int face_counter = 0;

		for (int j = 0; j < input_filePathNames.size(); j++) {

			Mat image_gray = imread(input_filePathNames[j], CV_LOAD_IMAGE_COLOR);

			if (fd.detectFaces(image_gray, faces_r, leftEyes_r, rightEyes_r)) {

				for (vector<Rect>::const_iterator face_r = faces_r.begin(), eyeL_r = leftEyes_r.begin(), eyeR_r = rightEyes_r.begin();
					face_r != faces_r.end(); face_r++, eyeL_r++, eyeR_r++) {

					Mat face_image = image_gray(*face_r).clone();

					Point leftEyeOnFrame((*eyeL_r).x + (*eyeL_r).width / 2, (*eyeL_r).y + (*eyeL_r).height / 2);
					Point rightEyeOnFrame((*eyeR_r).x + (*eyeR_r).width / 2, (*eyeR_r).y + (*eyeR_r).height / 2);

					//circle(frame, Point((*eyeL_r).x + (*eyeL_r).width / 2, (*eyeL_r).y + (*eyeL_r).height / 2), 2, Scalar(0, 0, 255), 2, CV_AA);
					//circle(frame, Point((*eyeR_r).x + (*eyeR_r).width / 2, (*eyeR_r).y + (*eyeR_r).height / 2), 2, Scalar(0, 0, 255), 2, CV_AA);

					Point leftEye((*eyeL_r).x - (*face_r).x + (*eyeL_r).width / 2, (*eyeL_r).y - (*face_r).y + (*eyeL_r).height / 2);
					Point rightEye((*eyeR_r).x - (*face_r).x + (*eyeR_r).width / 2, (*eyeR_r).y - (*face_r).y + (*eyeR_r).height / 2);

					//circle(face_image, leftEye, 2, Scalar(0, 0, 255), 2, CV_AA);
					//circle(face_image, rightEye, 2, Scalar(0, 0, 255), 2, CV_AA);

					cvtColor(face_image, face_image, CV_BGR2GRAY);

					Mat aligned_face;

					fa.align(face_image, aligned_face, leftEye, rightEye);

					Mat leftFace, rightFace;

					int leftX, leftY, rightX, rightY;
					int leftWidth, leftHeight, rightWidth, rightHeight;

					leftX = 0;
					leftY = 0;

					rightX = aligned_face.cols / 2;
					rightY = 0;

					leftWidth = aligned_face.cols / 2;
					leftHeight = aligned_face.rows;

					rightWidth = leftWidth;
					rightHeight = leftHeight;

					leftFace = aligned_face(Rect(leftX, leftY, leftWidth, leftHeight));
					rightFace = aligned_face(Rect(rightX, rightY, rightWidth, rightHeight));

					equalizeHist(aligned_face, aligned_face);
					equalizeHist(leftFace, leftFace);
					equalizeHist(leftFace, leftFace);

					int h = aligned_face.rows;
					int w = aligned_face.cols;
					int midX = w / 2;

					for (int y = 0; y<h; y++) {
						for (int x = 0; x<w; x++) {
							int v;
							if (x < w / 4) {
								v = leftFace.at<uchar>(y, x);
							}
							else if (x < w * 2 / 4) {
								int lv = leftFace.at<uchar>(y, x);
								int wv = aligned_face.at<uchar>(y, x);

								float f = (x - w * 1 / 4) / (float)(w*0.25f);
								v = cvRound((1.0f - f) * lv + (f)* wv);
							}
							else if (x < w * 3 / 4) {
								int rv = rightFace.at<uchar>(y, x - midX);
								int wv = aligned_face.at<uchar>(y, x);

								float f = (x - w * 2 / 4) / (float)(w*0.25f);
								v = cvRound((1.0f - f) * wv + (f)* rv);
							}
							else {
								v = rightFace.at<uchar>(y, x - midX);
							}
							aligned_face.at<uchar>(y, x) = v;
						}
					}

					Mat filtered = Mat(aligned_face.size(), CV_8U);
					bilateralFilter(aligned_face, filtered, 0, 20.0, 2.0);

					const double FACE_ELLIPSE_CY = 0.40;
					const double FACE_ELLIPSE_W = 0.50;
					const double FACE_ELLIPSE_H = 0.80;

					Mat mask = Mat(filtered.size(), CV_8U, Scalar(0));
					Point faceCenter = Point(filtered.cols / 2, cvRound(filtered.rows * FACE_ELLIPSE_CY));
					Size size = Size(cvRound(filtered.cols * FACE_ELLIPSE_W), cvRound(filtered.rows * FACE_ELLIPSE_H));
					ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);

					Mat final_face = Mat(filtered.size(), CV_8U, Scalar(128));
					filtered.copyTo(final_face, mask);

					string output_file = outputSubFolders[i].getPath() + to_string(face_counter) + ".jpg";
					imwrite(output_file, final_face);

					output_file_list.push_back(output_file);

					face_counter++;

				}








				//fd.detectFaces(image_gray, faces_r);		

				//for (vector<Rect>::const_iterator face_r = faces_r.begin(); face_r != faces_r.end(); face_r++) {

				//	Mat face_image = image_gray(*face_r);

				//	cvtColor(face_image, face_image, CV_BGR2GRAY);
				//	equalizeHist(face_image, face_image);

				//	resize(face_image, face_image, FACE_SIZE, 1.0, 1.0, INTER_CUBIC);

				//	Mat mask = Mat(face_image.size(), CV_8U, Scalar(0));
				//	Mat filtered = Mat(face_image.size(), CV_8U);
				//	bilateralFilter(face_image, filtered, 0, 20.0, 2.0);
				//	Point faceCenter = Point(face_image.size().width / 2, face_image.size().height / 2);
				//	Size size = Size(55, 70);
				//	ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);

				//	Mat res = Mat(face_image.size(), CV_8U, Scalar(128));
				//	filtered.copyTo(res, mask);

				//	string output_file = outputSubFolders[i].getPath() + to_string(face_counter) + ".jpg";
				//	imwrite(output_file, res);		

				//	output_file_list.push_back(output_file);

				//	face_counter++;
				//}
			}

		}

		string choice;

		while (!boost::algorithm::iequals(choice, "q")) {

			cout << "Silinecek hatalý fotoðraf" << "(" << outputSubFolders[i].getName() << "): ";

			cin >> choice;			

			int k = 0;

			for (vector<string>::iterator id = output_file_list.begin(); id != output_file_list.end(); id++) {

				if (boost::algorithm::iequals(*id, outputSubFolders[i].getPath() + choice + ".jpg")) {

					boost::filesystem::remove(*id);

					output_file_list.erase(output_file_list.begin() + k);

					break;
				}

				k++;
			}
		}

		list.open(output_folder + "/" + "list.txt", ios::app);

		for (vector<string>::iterator image = output_file_list.begin(); image != output_file_list.end(); image++) {

			list << *image << ";" << to_string(i) << ";" << outputSubFolders[i].getName() << endl;
		}

		list.close();
	}

	std::system("PAUSE");

	return 0;
}
