#include "Global.h"

void Global::ReadData(string path)
{
	fileName.clear();
	for (const auto& entry : experimental::filesystem::directory_iterator(path))
	{
		fstream fin;
		fin.open(entry.path());
		if (fin.is_open())
		{
			//them vao danh sach file
			string p = entry.path().string();
			fileName.push_back(p);
			string temp;
			//lay stt cua ten file
			int fileIndex = fileName.size() - 1;
			//doc dong dau (title)
			if (getline(fin, temp))
			{
				if (temp.length() != 0)
				{
					stringstream s(temp);
					string word;
					while (getline(s, word, ' '))
						trie.Insert(word, true, fileIndex);
				}
			}
			// ko phai title
			while (!fin.eof())
			{
				fin >> temp;
				trie.Insert(temp, false, fileIndex);
			}
		}
		//cout << entry.path() << endl;
		fin.close();
	}
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
