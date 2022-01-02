#include "header.h"

void scroll_dir(path p, bool end_dir) {
	static vector<path> fwd;
	int first = 0, last = -1, curr = 0;
	directory_iterator d_itr;
	vector<directory_entry> dir;
	struct termios initialrsettings, newrsettings;
	path temp;

	if (end_dir) {
		tcgetattr(fileno(stdin), &initialrsettings);
		//switch to canonical mode and echo mode
		newrsettings = initialrsettings;
		newrsettings.c_lflag &= ~ICANON;
		newrsettings.c_lflag &= ~ECHO;
		tcsetattr(fileno(stdin), TCSAFLUSH, &newrsettings);
	}

	while (true) {
		if (last == -1) {
			d_itr = directory_iterator(p);
			dir = { directory_entry("."), directory_entry("..") };
			for (auto e : d_itr) {
				//if (i < first)	continue;
				dir.push_back(e);
				//i++;
				//if (i == last)	break;
			}
			first = 0;
			curr = 0;
			last = min((int)dir.size(), SCREEN_HT - 3);
		}

		cout << CLRSCR;
		cout << "File Name\t\tFile Size\tPermission\tUserID\t    GroupID\tLast Modified\n"
			<< string(112, '-')
			<< endl;

		for (int i = first; i < last; i++)
			print_dir_ent(dir[i], i == curr);

		switch (cin.get()) {
		case 27:
			cin.get();
			switch (cin.get()) {
			case 'A':
				curr += dir.size() - 2;
			case 'B':
				curr = (curr + 1) % dir.size();
				if (curr < first) {
					first = curr;
					last = min(first + SCREEN_HT - 3, (int)dir.size());
				}
				if (curr >= last) {
					last = curr + 1;
					first = max(last - SCREEN_HT + 3, 0);
				}
				break;
			case 'C':
				if (!fwd.empty()) {
					temp = fwd.back();
					fwd.pop_back();
					dir.clear();
					last = -1;
					print_dir(temp, false);
				}
				break;
			case 'D':
				if (!end_dir) {
					fwd.push_back(absolute(dir[0].path()));
					return;
				}
			}
			break;
		case 104:
		case 72:
			fwd.clear();
			dir.clear();
			last = -1;
			print_dir(home, false);
			break;
		case 127:
			curr = 1;
		case 10:
			if (curr == 0)
				break;
			temp = dir[curr].path();
			dir.clear();
			last = -1;
			if (dir[curr].is_directory()) {
				fwd.clear();
				print_dir(temp, false);
			}
			else if (dir[curr].is_regular_file()) {
				pid_t pid;
				int status;
				auto str = temp.filename().string();
				char* argv[] = { "nano", new char[str.length() + 1], NULL };
				strcpy(argv[1], str.c_str());
				status = posix_spawn(&pid, "/bin/nano", NULL, NULL, argv, environ);
				!status and waitpid(pid, &status, 0);
			}
			else cout << "UNKNOWN FILE TYPE!! COULDN'T OPEN\n";
			break;
		case ':':
			last = -1;
			p = absolute(p);
			current_path(p);
			while (cmd_mode());
			current_path(p);
			break;
		}
	}

	if (end_dir)
		tcsetattr(fileno(stdin), TCSANOW, &initialrsettings);
}

void print_dir(path p, bool end_dir) {
	auto initial = current_path();
	p = absolute(p);
	current_path(p);
	scroll_dir(p, end_dir);
	current_path(initial);
}