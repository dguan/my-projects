#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>

using namespace std;

namespace {
    typedef std::unordered_map<char, char> PatternMap;
    typedef std::unordered_set<char> LookupTable;
    
    LookupTable pat_char_set;
    LookupTable wc_uniq_char_set;
    LookupTable all_nz_char_set;
    LookupTable digits_set;

    char wc_char = '?';
    char wc_char_nz = '~';
    string wc_uniq_chars = "QRSTUVWXYZ";
    string wc_uniq_chars_nz = "!@#$%^&*(";
    string wc_uniq_chars_nz_regex_str = "!@#$%\\^&*("; // The regex string of these chars

    string pat_chars = "abcdefghijklmnopqrstuvwxyz";
    string pat_chars_nz = "ABCDEFGH";
};

string find_availables(const PatternMap& x_map, char x_char, char expected)
{
    string results;
    PatternMap::const_iterator citr = x_map.find(x_char);
    if(expected==0)
    {
        char start_ch = (all_nz_char_set.find(x_char)!=all_nz_char_set.cend()) ? '1' : '0';
        if(citr->second == 0)
        {
            for(char c = start_ch; c<='9'; ++c)
            {
                bool not_found = true;
                for(auto& x : x_map)
                {
                    if(x.second == c)
                    {
                        not_found = false;
                        break;
                    }
                }
                if(not_found)
                    results += c;
            }
        }
        else
            results += citr->second;
    }
    else
    {
        if(citr->second == 0)
        {
            bool found = false;
            for(auto& x : x_map)
            {
                if(x.second == expected)
                {
                    found = true;
                    break;
                }
            }
            if(found)
                return results;
            else
                results += '\0';
        }
        else if(expected != citr->second)
            return results;
        else
            results += '\0';
    }
    return results;
}

vector<string> gen_num(const PatternMap& pat_map, const PatternMap& wc_map, const string& num_pat)
{
    vector<string> results;
    string leading_str;
    string availables;
    
    unsigned int pat_pos = 0;
    while(pat_pos < num_pat.length())
    {
        if(digits_set.find(num_pat[pat_pos]) != digits_set.cend())
        {
            leading_str += num_pat[pat_pos++];
            continue;
        }
        else if(num_pat[pat_pos]==wc_char || num_pat[pat_pos]==wc_char_nz)
        {
            availables = num_pat[pat_pos]==wc_char ? "0123456789" : "123456789";
            break;
        }
        else
        {
			availables = std::move(find_availables(pat_char_set.find(num_pat[pat_pos]) != pat_char_set.cend() ? pat_map : wc_map, num_pat[pat_pos], 0));
            break;
        }
    }
    if(num_pat.length()-pat_pos == 0)
    {
		if (availables.length()==0)
			results.emplace_back(leading_str);
		else
			for (auto ch : availables)
				results.emplace_back(leading_str + ch);
    }
	else if (num_pat[pat_pos]==wc_char || num_pat[pat_pos] == wc_char_nz)
	{
		vector<string> sub_pat_strs = std::move(gen_num(pat_map, wc_map, num_pat.substr(pat_pos + 1)));
		if (sub_pat_strs.size())
			for (auto ch : availables)
				for (auto& sps : sub_pat_strs)
					results.emplace_back(leading_str + ch + sps);
	}
    else
    {
		vector<string> sub_pat_strs;
		bool is_pat_char = pat_char_set.find(num_pat[pat_pos]) != pat_char_set.cend();
		PatternMap x_map = is_pat_char ? pat_map : wc_map;
        for(auto ch : availables)
        {
			x_map[num_pat[pat_pos]] = ch;
			if (is_pat_char)
				sub_pat_strs = std::move(gen_num(x_map, wc_map, num_pat.substr(pat_pos+1)));
			else
				sub_pat_strs = std::move(gen_num(pat_map, x_map, num_pat.substr(pat_pos+1)));
            for(auto& sps : sub_pat_strs)
                results.emplace_back(leading_str + ch + sps);
        }
    }
    return results;
}

bool check_validity(PatternMap& pat_map, PatternMap& wc_map, const string& rslt_pat, const string& rslt_str)
{
    unsigned int pat_pos = 0;
    while(pat_pos < rslt_pat.length())
    {
        if(digits_set.find(rslt_pat[pat_pos]) != digits_set.cend())
        {
            if(rslt_pat[pat_pos] != rslt_str[pat_pos])
                return false;
            else
            {
                ++ pat_pos;
                continue;
            }
        }
        else if(rslt_pat[pat_pos]==wc_char || rslt_pat[pat_pos]==wc_char_nz)
        {
            if(rslt_str[pat_pos]=='0' && rslt_pat[pat_pos]==wc_char_nz)
                return false;
            else
            {
                ++ pat_pos;
                continue;
            }
        }
        else if(all_nz_char_set.find(rslt_pat[pat_pos]) != all_nz_char_set.cend())
        {
            if(rslt_str[pat_pos] == '0')
                return false;
            else
                break;
        }
        else
            break;
    }
    
    if(rslt_pat.length()-pat_pos == 0)
        return true;
    else
    {
        bool is_pat_char = pat_char_set.find(rslt_pat[pat_pos]) != pat_char_set.cend();
        PatternMap *p_x_pat_map = is_pat_char ? &pat_map : &wc_map;
        if(find_availables(*p_x_pat_map, rslt_pat[pat_pos], rslt_str[pat_pos]).length() == 0 )
            return false;
        else
        {
            (*p_x_pat_map)[rslt_pat[pat_pos]] = rslt_str[pat_pos];
            return check_validity(pat_map, wc_map, rslt_pat.substr(pat_pos+1), rslt_str.substr(pat_pos+1)); 
        }
    }
}

static queue<string> g_results_queue;
static mutex g_queue_mutex;
static atomic<int> g_active_thrds_cnt(0);

void get_range_results(const vector<string>& n1_strs, int data_low, int data_high, const PatternMap& pat_map, \
	const PatternMap& wc_map, const string& num1_pat, const string& num2_pat, const string& rslt_pat, char op)
{
	unordered_map<char, std::function<int(int, int)> > calcu_it;
	calcu_it['+'] = [&](int a, int b) { return a + b; };
	calcu_it['-'] = [&](int a, int b) { return a - b; };
	calcu_it['*'] = [&](int a, int b) { return a*b; };
	calcu_it['/'] = [&](int a, int b) { return a / b; };
	calcu_it['%'] = [&](int a, int b) { return a%b; };

	for (int idx = data_low; idx < data_high; ++idx)
	{
		PatternMap n1_pat_map = pat_map;
		PatternMap n1_wc_map = wc_map;
		for (unsigned int i = 0; i<num1_pat.length(); ++i)
		{
			if (pat_char_set.find(num1_pat[i]) != pat_char_set.cend())
				n1_pat_map[num1_pat[i]] = n1_strs[idx][i];
			else if (wc_uniq_char_set.find(num1_pat[i]) != wc_uniq_char_set.cend())
				n1_wc_map[num1_pat[i]] = n1_strs[idx][i];
			else
				continue;
		}

		vector<string> n2_strs = std::move(gen_num(n1_pat_map, n1_wc_map, num2_pat));
		for (auto& n2 : n2_strs)
		{
			int y = atoi(n2.c_str());
			if ((op == '/' || op == '%') && y == 0)
				continue;
			int rslt_i = calcu_it[op](atoi(n1_strs[idx].c_str()), y);

			stringstream oss;
			oss << rslt_i;
			string rslt_str = oss.str();
			if (rslt_str.length() > rslt_pat.length())
				continue;
			else if (rslt_str.length() < rslt_pat.length())
				rslt_str = string(rslt_pat.length() - rslt_str.length(), '0') + rslt_str;

			PatternMap n2_pat_map = n1_pat_map;
			PatternMap n2_wc_map = n1_wc_map;
			for (unsigned int i = 0; i < num2_pat.length(); ++i)
			{
				if (pat_char_set.find(num2_pat[i]) != pat_char_set.cend())
					n2_pat_map[num2_pat[i]] = n2[i];
				else if (wc_uniq_char_set.find(num2_pat[i]) != wc_uniq_char_set.cend())
					n2_wc_map[num2_pat[i]] = n2[i];
				else
					continue;
			}
			if (check_validity(n2_pat_map, n2_wc_map, rslt_pat, rslt_str) == true)
			{
				std::lock_guard<std::mutex> queue_gurad(g_queue_mutex);
				g_results_queue.emplace(n1_strs[idx] + ' ' + op + ' ' + n2 + " = " + rslt_str);
			}
		}
	}
	-- g_active_thrds_cnt;
}

bool init_PatternMaps(PatternMap& pat_map, PatternMap& wc_map, const string& n1_pat, const string& n2_pat, const string& rslt_pat)
{
	int in_pat_char_cnt = 0;
	int in_wc_uniq_char_cnt = 0;
    for(auto ch : n1_pat + n2_pat + rslt_pat)
    {
        if(wc_uniq_char_set.find(ch) != wc_uniq_char_set.cend())
            wc_map[ch] = 0;
		else if ( pat_char_set.find(ch)!=pat_char_set.cend() && pat_map.find(ch)==pat_map.cend() )
        {
			pat_map[ch] = 0;
			++ in_pat_char_cnt;
        }
		else if (wc_uniq_char_set.find(ch) != wc_uniq_char_set.cend() && wc_map.find(ch) == pat_map.cend())
		{
			wc_map[ch] = 0;
			++in_wc_uniq_char_cnt;
		}
		else
            continue;
	}
	if (in_pat_char_cnt > 10 || in_wc_uniq_char_cnt > 10)
	{
		pat_map.clear();
		wc_map.clear();
		return false;
	}
	return true;
}

int main()
{
    ////////////////////////////////////////////////////////////////////
    //                                                                //
    // g++ 8.4 does not support regex yet, so regex related code are  //
    // commented out, these lines works in VS 2013.                   //
    //                                                                //
    ////////////////////////////////////////////////////////////////////

    {	// LookupTables initialization
        vector<LookupTable*> p_tbls{ &digits_set, &wc_uniq_char_set,              &pat_char_set,          &all_nz_char_set };
        vector<string>    init_strs{ "0123456789",      wc_uniq_chars+wc_uniq_chars_nz, pat_chars+pat_chars_nz, pat_chars_nz+wc_uniq_chars_nz };
        for (unsigned int i = 0; i < p_tbls.size(); ++i)
            for (auto ch : init_strs[i])
                p_tbls[i]->insert(ch);
    }
    string n1_pat, n2_pat, rslt_pat;
    char op, equal;
    PatternMap pat_map, wc_map;

    string original_expression;
    
    string num_regex_str = string("[\\w|") + wc_char + wc_char_nz + wc_uniq_chars_nz_regex_str + "]+";
    string input_regex_str = "^\\s*" + num_regex_str + "\\s+[+*/%-]\\s+" + num_regex_str + "\\s+=\\s+" + num_regex_str + "\\s*$";
    
    while (1)
    {
        cout << "Please input an expression(CR to quit): ";
        std::getline(std::cin, original_expression);
        if(original_expression.length() == 0)
            break;

        if (!regex_match(original_expression, regex(input_regex_str)))
        {
            cout << "There are some errors in the expression, please retry." << endl;
            continue;
        }
        
        std::istringstream iss(original_expression);
        iss >> n1_pat >> op >> n2_pat >> equal >> rslt_pat;
        if( ! init_PatternMaps(pat_map, wc_map, n1_pat, n2_pat, rslt_pat) )
        {
            cout << "There are more than 10 PatternMap chars in the expression, please try again.";
            continue;
        }

        auto t0 = chrono::high_resolution_clock::now();
        
        const int MaxThreads = 32; // Maximum number of threads
        const int TimeDelay = 100; // Time delay in milliseconds

        vector<string> n1_strs = std::move(gen_num(pat_map, wc_map, n1_pat));
        int data_step = n1_strs.size() <= MaxThreads ? 1 : (n1_strs.size() + MaxThreads) / MaxThreads;
        int total_solutions = 0;
        vector<std::thread> thrds;
        unsigned int low = 0;
        while (low < n1_strs.size())
        {
            std::lock_guard<std::mutex> create_guard(g_queue_mutex);
            int high = low + data_step <= n1_strs.size() ? low+data_step : n1_strs.size();
            thrds.emplace_back(std::thread(&get_range_results, n1_strs, low, high, pat_map, wc_map, n1_pat, n2_pat, rslt_pat, op));
            low = high;
            ++ g_active_thrds_cnt;
        }
        while (g_active_thrds_cnt)
        {
            if(g_queue_mutex.try_lock())
            {
                while ( ! g_results_queue.empty() )
                {
                    cerr << endl << g_results_queue.front();
                    g_results_queue.pop();
                    ++total_solutions;
                }
                g_queue_mutex.unlock();
            }
            cerr << '.';
            this_thread::sleep_for(std::chrono::milliseconds(TimeDelay));
        }
        while (!g_results_queue.empty())
        {
            cout << endl << g_results_queue.front();
            g_results_queue.pop();
            ++total_solutions;
        }

        auto t1 = chrono::high_resolution_clock::now();

        for (unsigned int i = 0; i < thrds.size(); ++i)
            thrds[i].join();

        if (total_solutions)
            cout << endl << "Total " << total_solutions << " solutions for '" << original_expression << "'" << endl;
        else
            cout << endl << "No solution for '" << original_expression << "'" << endl;
        cout << "Time spent : " << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count()/1000.0 << "seconds" << endl;
        
        pat_map.clear();
        wc_map.clear();
    }
    cout << "Thanks for using, bye!" << endl;
    return 0;
}
