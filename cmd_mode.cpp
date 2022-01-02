#include "header.h"

bool cmd_mode() {
	auto p = current_path();
	cout << p.string() << ':' << flush;
	string cmd;
	bool line = true;
	char c;
	while (line) {
		switch (c = cin.get()) {
		case 127:
			if (!cmd.empty()) {
				cmd.pop_back();
				cout << "\b \b" << flush;
			}
			break;
		case 10:
			line = false;
			cout << endl;
			break;
		case 27:
			line = false;
			return false;
		default:
			cout << c << flush;
			cmd.push_back(c);
		}
	}
	istringstream ssin(cmd);
	vector<string> argv;
	string ins;
	ssin >> ins;
	char ch;
	while (ssin >> ch) {
		argv.push_back("");
		if (ch != '\'' and ch != '\"') {
			ssin.unget();
			ssin >> argv.back();
		}
		else
			getline(ssin, argv.back(), ch);
	}
	int n = argv.size() - 1;
	fstream f;
	switch (ins[0]) {
	case 'c':
		if (ins == "copy") {
			const auto copyOptions = copy_options::overwrite_existing | copy_options::recursive;
			error_code ec;
			for (int i = 0; i < n; i++)
				copy(argv[i], argv[n]+"/"+ argv[i], copyOptions, ec);
		}
		else if (ins == "create_file") {
			f.open(((argv.size() == 2) ? argv[1] + "/" : "") + argv[0], ios::app);
			f.close();
		}
		else if (ins == "create_dir")
			create_directory(((argv.size() == 2) ? argv[1] + "/" : "") + argv[0]);
		break;
	case 'd':
		if (ins == "delete_file" or ins == "delete_dir")
			remove_all(argv[0]);
	case 'g':
		if (ins == "goto")
			current_path(argv[0]);
		break;
	case 'm':
		if (ins == "move")
			for (int i = 0; i < n; i++)
				rename(argv[i], argv[n] + "/" + argv[i]);
		break;
	case 'r':
		if (ins == "rename")
			rename(argv[0], argv[1]);
		break;
	case 's':
		if (ins == "search") {
			recursive_directory_iterator r_d_itr(current_path());
			for (auto e : r_d_itr)
				if (e.path().stem() == argv[0])
					print_dir_ent(e,false);
		}
	}
	return true;
}
