#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

using namespace std;

void findHelp() {
    cerr << "incorrect command, use ./mkglist -h to get help." << endl;
}

void printInfo()
{
    cout << "Usage: ./mkglist [-f file] | -h" << endl;
    cout << endl;
    cout << "The details about options & parameters:" << endl;
    cout << "   -f   file       " << "A must. Specify the input file." << endl;
    cout << "   -h              " << "Not must. Find help on how to use this command." << endl;
    cout << endl;
    cout << "You can sepcify the output file via pipeline." << endl
    << "Like ./mkglist -f users.xml >> output.txt"
    << endl << endl;
    cout << "For further information, please contact me at chinylv@gmail.com." << endl;
}

string clearSpace(string& input)
{
    size_t cur;
    while(string::npos != (cur = input.find(" "))) input.replace(cur, 1, "");
    while(string::npos != (cur = input.find("\n"))){
        input.replace(cur, 1, ",");
    }
    return input;
}


vector<string> split(string str, string pattern)
{
    vector<string> result;
    str += pattern;
    size_t pos, size = str.size();

    for (size_t i = 0; i < size; i++) {
        pos = str.find(pattern,i);
        if (pos < size) {
            string s = str.substr(i,pos-i);
            result.push_back(s);
            i = pos+pattern.size()-1;
        }
    }
    return result;
}


bool isGroupDefined(const vector<string>& titles, const string& group) {
    bool ret = false;
    for (size_t i = 0; i < titles.size(); i++) {
        if (titles[i] == group) {
            ret = true;
            break;
        }
    }
    return ret;
}


int main(int argc, char** argv)
{
    char ch;

    bool fFlag = false, hFlag = false;
    string fParam;

    while ((ch = getopt(argc, argv, "f:h")) != -1) {
        switch (ch) {
            case 'f':
                if (fFlag) {
                    cerr << "multiple option -f" << endl;
                    return -1;
                }
                fFlag = true;
                fParam = string(optarg);
                if (fParam.rfind(".xml") == fParam.npos || fParam == ".xml" || fParam == "") {
                    cerr << "invalid input file, only xml files are allowed" << endl;
                    return -1;
                }
                break;
            case 'h':
                if (hFlag) {
                    cerr << "multiple option -h" << endl;
                    return -1;
                }
                hFlag = true;
                break;
            case '?':
                findHelp();
                return -1;
        }
    }

    if (hFlag && fFlag) {
        findHelp();
        return -1;
    } else if (hFlag) {
        printInfo();
        return 1;
    }

    if (!fFlag) {
        cerr << "input files must be indicated, "
        << "use ./mkglist -h to get help."
        << endl;
        return -1;
    }

    xmlDocPtr doc;
    xmlNodePtr curNode;
    xmlNodePtr childNode;

    xmlKeepBlanksDefault(0);
    doc = xmlReadFile(fParam.c_str(), "UTF-8", XML_PARSE_RECOVER); // open a xml doc.
    curNode = xmlDocGetRootElement(doc); // get root element.

    if (curNode == NULL) {
        cerr << "open file failed." << endl;
        xmlFreeDoc (doc);
        return -1;
    }

    if (xmlStrcmp(curNode->name, (const xmlChar*)"users")) {
        cerr << "check rootElement failed." << endl;
        xmlFreeDoc (doc);
        return -1;
    }

    string uname, group;
    vector<string> titles, groups;
    bool defined = false;
    vector< vector<string> > vecnames;
    map< string, int > vecmap;

    for (curNode = curNode->children; curNode != NULL; curNode = curNode->next) {
        uname.clear();
        group.clear();
        for (childNode = curNode->children; childNode != NULL; childNode = childNode->next) {
            if (!xmlStrcmp(childNode->name, (const xmlChar*) "username")
                && xmlStrcmp(xmlNodeGetContent(childNode), (const xmlChar*) "")) {
                uname = string((char*) xmlNodeGetContent(childNode));
            } else if (!xmlStrcmp(childNode->name, (const xmlChar*) "group")
                && xmlStrcmp(xmlNodeGetContent(childNode), (const xmlChar*) "")) {
                group = string((char*) xmlNodeGetContent(childNode));
                clearSpace(group);

                groups = split(group, string(","));
            }
        } // end for

        for (size_t i = 0; i < groups.size(); i++) {
            defined = isGroupDefined(titles, groups[i]);
            if (!defined) {
                titles.push_back(groups[i]);
            }
            if (!uname.empty() && !groups.empty()) {
                if (defined) {
                    vector<string> tvec = vecnames[vecmap[groups[i]] - 1];
                    tvec.push_back(uname);
                    vecnames[vecmap[groups[i]] - 1] = tvec;
                } else {
                    vector<string> tvec = vector<string>();
                    tvec.push_back(uname);
                    vecnames.push_back(tvec);
                    vecmap[groups[i]] = (int)vecnames.size();
                }
            }
        } // end for
    } // end for

    for (size_t i = 0; i < titles.size(); i++) {
        cout << titles[i] << "  (";
        vector<string> nlist = vecnames[vecmap[titles[i]] - 1];
        size_t size = nlist.size();
        if (size > 0) cout << nlist[0];
        for (size_t j = 1; j < size; j++) {
            cout << " " << nlist[j];
        }
        cout << ") -" << endl;
    }

    xmlFree(curNode);
    xmlFreeDoc(doc);
    return 0;
}
