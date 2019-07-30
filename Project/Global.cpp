#include "Global.h"
#include<iterator>
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
		//cout << "Entry: " << filename << endl;
		//Trie temp;
		//trie.push_back(temp);
		int length = 0;
		Count++;
		//std::cout << filename << '\n';
		if (Count == 1000) break;
		filename = path + filename;

		fileName.push_back(filename);
		numberList.push_back({});
		priceList.push_back({});

		int index = fileName.size() - 1;
		ifstream data;
		data.open(filename);
		string word, kor;

		string temp;
		temp.clear();
		//vector<string> line;
		//doc dong dau (title)
		while (getline(data, word))
		{
			if (word.length() != 0)
			{
				for (char i : word)
				{
					i = tolower(i);
					if (('a' <= i && i <= 'z') || ('0' <= i && i <= '9'))
						temp.push_back(i);
					else if ((i == '#' || i == '$') && temp.size() == 0)
						temp.push_back(i);
					else
					{
						if (temp.size() != 0)
						{
							trie.Insert(temp, true, index);
							//cout << temp<< " " << filename << endl;
							//trie.Insert(temp, true, index);
							//line.push_back(temp);
							oki.addToTF(temp, index);
							/*
							if (hasNumberPrefix(temp)) {
								string kor("");
								for (int i = 0; i < (int)temp.length(); ++i)
									if (isdigit(temp[i]))
										kor += temp[i];
									else break;
								while (!kor.empty() && kor[0] == '0') kor.erase(0, 1);
								numberList[index].push_back(stoll(temp));
								trie.Insert(to_string(stoll(temp)), true, index);
								oki.addToTF(to_string(stoll(temp)), index);
							} */
							if (hasNumberPrefix(temp)) {
								kor = getNumber(temp);
								numberList[index].push_back(kor);
								trie.Insert(kor, true, index);
								oki.addToTF(kor, index);
							}
							if (hasPricePrefix(temp)) {
								/*
								string tmp = temp; tmp.erase(0, 1);
								priceList[index].push_back(stoll(tmp));
								trie.Insert("$"+to_string(stoll(tmp)), true, index);
								oki.addToTF("$"+to_string(stoll(tmp)), index); */
								temp.erase(0, 1);
								kor = getNumber(temp);
								priceList[index].push_back(kor);
								trie.Insert("$"+kor, true, index);
								oki.addToTF("$"+kor, index);
								
							}
							length++;
							temp.clear();
						}
					}
				}
				if (temp.size() != 0) {
					trie.Insert(temp, true, index), length++, oki.addToTF(temp, index);
					if (hasNumberPrefix(temp)) {
						kor = getNumber(temp);
						numberList[index].push_back(kor);
						trie.Insert(kor, true, index);
						oki.addToTF(kor, index);
					}
					if (hasPricePrefix(temp)) {
						/*
						string tmp = temp; tmp.erase(0, 1);
						priceList[index].push_back(stoll(tmp));
						trie.Insert("$"+to_string(stoll(tmp)), true, index);
						oki.addToTF("$"+to_string(stoll(tmp)), index); */
						temp.erase(0, 1);
						kor = getNumber(temp);
						priceList[index].push_back(kor);
						trie.Insert("$" + kor, true, index);
						oki.addToTF("$" + kor, index);

					}
				}
				temp.clear();

				//Reparse one more time to read "5,000" or "$5,000"
				for (char i : word) {
					i = tolower(i);
					if (('a' <= i && i <= 'z') || ('0' <= i && i <= '9')) //"5,000gb"
						temp.push_back(i);
					else if ((i == '$') && temp.size() == 0)
						temp.push_back(i);
					else if (i == ',') temp.push_back(i); //Chap nhan dau phay
					else
					{
						if (temp.size() != 0)
						{
							bool hasComma = 0;
							for (auto it=temp.begin(); it!=temp.end(); ++it)
								if (*it == ',') {
									hasComma = 1;
									temp.erase(it);
									if (temp.empty()) break;
									it = temp.begin();
								}

							if (hasComma && (hasNumberPrefix(temp) || hasPricePrefix(temp))) {
								//Co nen add "5,000mb"->"5000mb" vao Trie hay khong?
								//trie.Insert(temp, true, index);
								//oki.addToTF(temp, index); 
								if (hasNumberPrefix(temp)) {
									kor = getNumber(temp);
									if (kor != "") {
										numberList[index].push_back(kor);
										trie.Insert(kor, true, index);
										oki.addToTF(kor, index);
									}
								}
								else
									if (hasPricePrefix(temp)) {
										/*
										string tmp = temp; tmp.erase(0, 1);
										priceList[index].push_back(stoll(tmp));
										trie.Insert("$"+to_string(stoll(tmp)), true, index);
										oki.addToTF("$"+to_string(stoll(tmp)), index); */
										temp.erase(0, 1);
										kor = getNumber(temp);
										priceList[index].push_back(kor);
										trie.Insert("$" + kor, true, index);
										oki.addToTF("$" + kor, index);

									}
								}
							}
							
							//cout << temp<< " " << filename << endl;
							//trie.Insert(temp, true, index);
							//line.push_back(temp);
							temp.clear();
						}
					}
				//LAZY FOR REPARSING LAST TOKEN
				//if (temp.length()!=0)
				temp.clear();
				break;
			} //if
		}
		//cout << temp << " " << filename << endl;
		//if (temp.size() != 0) trie.Insert(temp, true, index);
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
				else if ((i == '#' || i == '$') && temp.size() == 0)
					temp.push_back(i);
				else //ky tu khac ngoai chu cai va digit
				{
					if (temp.size() != 0)
					{
						//TODO Hỏi ý kiến Duy (đã sửa true thành false)
						trie.Insert(temp, false, index);  //WATCHOUT THIS!
						//cout << temp<< " " << filename << endl;
						//trie.Insert(temp, true, index);
						//line.push_back(temp);
						oki.addToTF(temp, index);
						if (hasNumberPrefix(temp)) {
							kor = getNumber(temp);
							numberList[index].push_back(kor);
							trie.Insert(kor, false, index);
							oki.addToTF(kor, index);
						}
						if (hasPricePrefix(temp)) {
							/*
							string tmp = temp; tmp.erase(0, 1);
							priceList[index].push_back(stoll(tmp));
							trie.Insert("$"+to_string(stoll(tmp)), true, index);
							oki.addToTF("$"+to_string(stoll(tmp)), index); */
							temp.erase(0, 1);
							kor = getNumber(temp);
							priceList[index].push_back(kor);
							trie.Insert("$" + kor, false, index);
							oki.addToTF("$" + kor, index);

						}
						length++;
						temp.clear();
					}
				}
			}
			if (temp.size() != 0) {
				trie.Insert(temp, false, index), length++, oki.addToTF(temp, index);
				if (hasNumberPrefix(temp)) {
					kor = getNumber(temp);
					numberList[index].push_back(kor);
					trie.Insert(kor, false, index);
					oki.addToTF(kor, index);
				}
				if (hasPricePrefix(temp)) {
					/*
					string tmp = temp; tmp.erase(0, 1);
					priceList[index].push_back(stoll(tmp));
					trie.Insert("$"+to_string(stoll(tmp)), true, index);
					oki.addToTF("$"+to_string(stoll(tmp)), index); */
					temp.erase(0, 1);
					kor = getNumber(temp);
					priceList[index].push_back(kor);
					trie.Insert("$" + kor, false, index);
					oki.addToTF("$" + kor, index);

				}
			}
			temp.clear();

			//Reparse one more time to read "5,000" or "$5,000"
			for (char i : word) {
				i = tolower(i);
				if (('a' <= i && i <= 'z') || ('0' <= i && i <= '9')) //"5,000gb"
					temp.push_back(i);
				else if ((i == '$') && temp.size() == 0)
					temp.push_back(i);
				else if (i == ',') temp.push_back(i); //Chap nhan dau phay
				else
				{
					if (temp.size() != 0)
					{
						bool hasComma = 0;
						for (auto it = temp.begin(); it != temp.end(); ++it)
							if (*it == ',') {
								hasComma = 1;
								temp.erase(it);
								if (temp.empty()) break;
								it = temp.begin();
							}

						if (hasComma && (hasNumberPrefix(temp) || hasPricePrefix(temp))) {
							//Co nen add "5,000mb"->"5000mb" vao Trie hay khong?
							//trie.Insert(temp, true, index);
							//oki.addToTF(temp, index); 
							if (hasNumberPrefix(temp)) {
								kor = getNumber(temp);
								if (kor != "") {
									numberList[index].push_back(kor);
									trie.Insert(kor, false, index);
									oki.addToTF(kor, index);
								}
							}
							else
								if (hasPricePrefix(temp)) {
									/*
									string tmp = temp; tmp.erase(0, 1);
									priceList[index].push_back(stoll(tmp));
									trie.Insert("$"+to_string(stoll(tmp)), true, index);
									oki.addToTF("$"+to_string(stoll(tmp)), index); */
									temp.erase(0, 1);
									kor = getNumber(temp);
									priceList[index].push_back(kor);
									trie.Insert("$" + kor, false, index);
									oki.addToTF("$" + kor, index);

								}
						}

						//cout << temp<< " " << filename << endl;
						//trie.Insert(temp, true, index);
						//line.push_back(temp);
						temp.clear();
					}
				}
			}
			//LAZY FOR LAST REPARSE TOKEN.
			//if (temp.length()!=0)
			temp.clear();
		}
		//cout << temp << " " << filename << endl;
		if (temp.size() != 0) {
			trie.Insert(temp, false, index), length++, oki.addToTF(temp, index);
			kor = getNumber(temp);
			if (kor != "") {
				numberList[index].push_back(kor);
				trie.Insert(kor, false, index);
				oki.addToTF(kor, index);
			}
			if (hasPricePrefix(temp)) {
				/*
				string tmp = temp; tmp.erase(0, 1);
				priceList[index].push_back(stoll(tmp));
				trie.Insert("$"+to_string(stoll(tmp)), true, index);
				oki.addToTF("$"+to_string(stoll(tmp)), index); */
				temp.erase(0, 1);
				kor = getNumber(temp);
				priceList[index].push_back(kor);
				trie.Insert("$" + kor, false, index);
				oki.addToTF("$" + kor, index);

			}
		}
		temp.clear();
		
		//set to okapi table
		oki.addToLengthTable(index, length);

		//trie.push_back(temp);
		//cout << "file " << fileName[fileName.size() - 1] << endl;
		data.close();
		sort(numberList[index].begin(), numberList[index].end(), cmpNumber);
		unique(numberList[index].begin(), numberList[index].end());
		sort(priceList[index].begin(), priceList[index].end(), cmpNumber);
		unique(priceList[index].begin(), priceList[index].end());

	}
	fin.close();
	//string listFile = path + "___index.txt"
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
	TotalDoc = Count;
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

QueryAnswer And(QueryAnswer a, QueryAnswer b)
{	
	QueryAnswer temp;
	if (a.empty() || b.empty())
		return temp;
	//find common element in 2 set preRes and temp
	QueryAnswer intersectSet;
	//set_intersection(a.begin(), a.end(), temp.begin(), b.end(),b.end(),inserter(intersectSet,intersectSet.begin()));
	//Fix
	set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(intersectSet, intersectSet.begin()));
	return intersectSet;
}
QueryAnswer Or(QueryAnswer a, QueryAnswer b)
{
	QueryAnswer temp;
	if (a.empty())
		return b;
	else if (b.empty())
		return a;
	else if (a.empty() && b.empty()) //return empty set
		return temp;
	for (QueryAnswer::iterator it = a.begin(); it != a.end(); ++it)//set automatically delete duplicates
		b.insert(*it);
	return b;
}
QueryAnswer Exclude(QueryAnswer a)
{
	QueryAnswer temp;
	// nothing to exclude
	Global *g = Global::GetInstance();
	int size = g->fileName.size();
	for (int i = 0; i < g->fileName.size(); ++i)
	{
		temp.emplace(i);
	}
	for (set<int>::iterator it = a.begin(); it != a.end(); ++it)
	{
		temp.erase(*it);
	}
	return temp;
}
