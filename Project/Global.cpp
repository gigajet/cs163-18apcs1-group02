#include "Global.h"

void Global::ReadData(string path)
{
	fileName.clear();
	//for (const auto& entry : experimental::filesystem::directory_iterator(path))
	//{
	//	fstream fin;
	//	fin.open(entry.path());
	//	if (fin.is_open())
	//	{
	//		//them vao danh sach file
	//		string p = entry.path().string();
	//		//fileName.push_back(p);
	//		string temp;
	//		//lay stt cua ten file
	//		int fileIndex = fileName.size() - 1;
	//		//doc dong dau (title)
	//		/*if (getline(fin, temp))
	//		{
	//			if (temp.length() != 0)
	//			{
	//				stringstream s(temp);
	//				string word;
	//				while (getline(s, word, ' '))
	//					trie.Insert(word, true, fileIndex);
	//			}
	//		}*/
	//		// ko phai title
	//		while (fin >> temp)
	//		{
	//			trie.Insert(temp, false, fileIndex);
	//		}
	//	}
	//	//cout << entry.path() << endl;
	//	fin.close();
	//}

	string listFile = path + "___index.txt";

	ifstream fin;
	fin.open(listFile);
	int Count = 0;
	string filename;
	while (getline(fin, filename))
	{
		//Trie temp;
		//trie.push_back(temp);
		Count++;
		//std::cout << filename << '\n';
		//if (Count == 2) break;
		filename = path + filename;

		fileName.push_back(filename);

		int index = fileName.size() - 1;
		ifstream data;
		data.open(filename);
		string word;

		string temp;
		temp.clear();
		//vector<string> line;
		//doc dong dau (title)
		if (getline(data, word))
		{
			if (word.length() != 0)
			{
				for (char i : word)
				{
					i = tolower(i);
					if (('a' <= i && i <= 'z') || ('0' <= i && i <= '9'))
						temp.push_back(i);
					else
					{
						if (temp.size() == 0) continue;
						trie.Insert(temp, true, index);
						//cout << temp<< " " << filename << endl;
						//trie.Insert(temp, true, index);
						//line.push_back(temp);
						temp.clear();
					}
				}
			}
		}
		//cout << temp << " " << filename << endl;
		if (temp.size() != 0) trie.Insert(temp, true, index);
			//trie.Insert(word, true, index);
			/*if (word.length() != 0)
			{
				stringstream s(word);
				string word;
				while (getline(s, word, ' '))
					trie.Insert(word, true, index);
			}*/

		//for (auto i : line)
		//{
		//	trie.Insert(i, true, index);
		//}

		temp.clear();

		while (getline(data, word))
		{
			for (auto i : word)
			{
				i = tolower(i);
				if (('a' <= i && i <= 'z') || ('0' <= i && i <= '9'))
					temp.push_back(i);
				else //ky tu khac ngoai chu cai va digit
				{
					if (temp.size() == 0) continue;
					//cout << temp << " " << filename << endl;
					trie.Insert(temp, false, index);
					temp.clear();
				}
			}
		}
		//cout << temp << " " << filename << endl;
		if (temp.size() != 0) trie.Insert(temp, false, index);
		temp.clear();
		//trie.push_back(temp);
		//cout << "file " << fileName[fileName.size() - 1] << endl;
		data.close();
	}
	fin.close();
	//string listFile = path + "___index.txt";

	//FILE* fin = fopen(listFile.c_str(), "r");

	//char filename[1000];
	//while (!feof(fin))
	//{
	//	fgets(filename, 1000, fin);
	//	//cout << filename << endl;
	//	if (filename[strlen(filename) - 1] == '\n')
	//		filename[strlen(filename) - 1] = '\0';
	//	char realpath[1000];
	//	string realName = path + filename;

	//	//strcat(realpath, filename);
	//	//cout << realName << endl;
	//	
	//	FILE* data = fopen(realName.c_str(), "r");
	//	if (data == NULL) 
	//	{
	//		cout << "Error!"; continue;
	//	}

	//	char buf[100000];
	//	while (!feof(data))
	//	{
	//		fgets(buf, 100000, data);
	//		string s(buf);
	//		trie.Insert(s);
	//	}

	//	fclose(data);
	//	
	//}

	//fclose(fin);

	cout << Count << endl;
}

Global* Global::GetInstance()
{
	if (instance == 0) instance = new Global();

	return instance;
}

Global* Global::instance = 0;

Global::Global() 
{
	fileName.clear();
}
