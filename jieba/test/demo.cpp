#include <stdio.h>
#include "cppjieba/Jieba.hpp"

using namespace std;

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

int main(int argc, char** argv) {

	// 

	//基于Trie树结构实现高效的词图扫描，生成句子中汉字所有可能成词情况所构成的有向无环图（DAG)

		//采用了动态规划查找最大概率路径, 找出基于词频的最大切分组合

		//对于未登录词，采用了基于汉字成词能力的HMM模型，使用了Viterbi算法


	cppjieba::Jieba jieba(DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH);

	FILE *fp_in = fopen("test.txt","r");
	FILE *fp_out = fopen("out.txt","w");
	if (fp_in == NULL )
	{
		printf("open file failed!");
		return 0;
	}

	vector<string> words;
	vector<cppjieba::Word> jiebawords;
	string s;
	string result;
	char *str_tmp = new char[1024];

	/*


	










	cout << "[demo] Lookup Tag for Single Token" << endl;
	const int DemoTokenMaxLen = 32;
	char DemoTokens[][DemoTokenMaxLen] = { "拖拉机", "CEO", "123", "。" };
	vector<pair<string, string> > LookupTagres(sizeof(DemoTokens) / DemoTokenMaxLen);
	vector<pair<string, string> >::iterator it;
	for (it = LookupTagres.begin(); it != LookupTagres.end(); it++) {
	it->first = DemoTokens[it - LookupTagres.begin()];
	it->second = jieba.LookupTag(it->first);
	}
	cout << LookupTagres << endl;



	cout << "[demo] Keyword Extraction" << endl;
	const size_t topk = 5;
	vector<cppjieba::KeywordExtractor::Word> keywordres;
	jieba.extractor.Extract(s, keywordres, topk);
	cout << s << endl;
	cout << keywordres << endl;

	*/

	while (fgets(str_tmp,1024,fp_in) )
	{
		if (strlen(str_tmp) > 0 && (str_tmp[strlen(str_tmp) - 1] == '\n' || str_tmp[strlen(str_tmp) - 1] == '\r'))
		{
			str_tmp[strlen(str_tmp) - 1] = '\0';
		}
		if (strlen(str_tmp) < 2)
		{
			continue;
		}

		// input text  注意：待分词的字符串可以是gbk字符串、utf-8字符串或者unicode 
		s = str_tmp; 
		fprintf(fp_out, "\nline_in:\t%s\n", str_tmp);

		// 
		// 精确模式 whthout HMM    MPSegment
		jieba.Cut(s, words, false);  // 默认是精确模式 
		fprintf(fp_out, "Without HMM:\t%s\n", limonp::Join(words.begin(), words.end(), "/").c_str());

		// 全模式 HMM    HMMSegment
		jieba.Cut(s, words, true);
		fprintf(fp_out, "With HMM:\t%s\n", limonp::Join(words.begin(), words.end(), "/").c_str());

		// 搜索引擎模式：该方法适合用于搜索引擎构建倒排索引的分词，粒度比较细
		// Query方法先使用Mix方法切词，对于切出来的较长的词再使用Full方法。
		jieba.CutForSearch(s, words);
		fprintf(fp_out, "CutForSearch:\t%s\n", limonp::Join(words.begin(), words.end(), "/").c_str());
		
		// CutAll  切出所有字典里的词语。
		jieba.CutAll(s, words);
		fprintf(fp_out, "CutAll:\t%s\n", limonp::Join(words.begin(), words.end(), "/").c_str());

		// tagging 
		vector<pair<string, string> > tagres;
		jieba.Tag(s, tagres);
		fprintf(fp_out, "Tagging:\t\n");
		for (vector<pair<string, string> >::iterator it = tagres.begin(); it != tagres.end(); it++)
		{
			fprintf(fp_out, "%s/%s ", it->first.c_str(), it->second.c_str());
		}
		fprintf(fp_out, "\n");


		// word offset 
		//jieba.CutForSearch(s, jiebawords, true);
		//fprintf(fp_out, "CutForSearch Word With Offset:\t%s", jiebawords.c_str());
		
		
		// 添加用户词典 
		//cout << "[demo] Insert User Word" << endl;
		//jieba.Cut("男默女泪", words);
		//cout << limonp::Join(words.begin(), words.end(), "/") << endl;
		//jieba.InsertUserWord("男默女泪");
		//jieba.Cut("男默女泪", words);
		//cout << limonp::Join(words.begin(), words.end(), "/") << endl;
		

	}

	if (fp_in) 
	{
		fclose(fp_in);
		fp_in = NULL;
	}
	if (fp_out)
	{
		fclose(fp_out);
		fp_out = NULL;
	}

	delete str_tmp;
	str_tmp = NULL;


	return EXIT_SUCCESS;
}
