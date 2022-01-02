#include "header.h"

string perms_linux(perms p) {
	ostringstream ss;
	ss << ((p & perms::owner_read) != perms::none ? "r" : "-")
		<< ((p & perms::owner_write) != perms::none ? "w" : "-")
		<< ((p & perms::owner_exec) != perms::none ? "x" : "-")
		<< ((p & perms::group_read) != perms::none ? "r" : "-")
		<< ((p & perms::group_write) != perms::none ? "w" : "-")
		<< ((p & perms::group_exec) != perms::none ? "x" : "-")
		<< ((p & perms::others_read) != perms::none ? "r" : "-")
		<< ((p & perms::others_write) != perms::none ? "w" : "-")
		<< ((p & perms::others_exec) != perms::none ? "x" : "-");
	return ss.str();
}

string file_size(double size) {
	ostringstream ss;
	string PreFix = "B";
	if (size > 1000) {
		size /= 1000;
		PreFix = "KB";
	} if (size > 1000) {
		size /= 1000;
		PreFix = "MB";
	} if (size > 1000) {
		size /= 1000;
		PreFix = "GB";
	} if (size > 1000) {
		size /= 1000;
		PreFix = "TB";
	}
	ss.precision(2);
	ss << fixed << size << " " << PreFix;
	return ss.str();
}

void print_dir_ent(directory_entry d, bool color) {
	ostringstream out;
	struct stat fileStat;
	auto filename = d.path().filename().string();
	filename = (filename.size() > ALIGN_FN) ?
		(filename.substr(0, ALIGN_FN - 3) + "...") :
		filename;
	out << filename << string(ALIGN_FN - out.str().length(), ' ');

	if (!d.is_directory())
		out << file_size(d.file_size());

	out << string(ALIGN_SZ - out.str().length(), ' ')
		<< ((d.is_directory()) ? 'd' : '-')
		<< perms_linux(d.status().permissions())
		<< string(ALIGN_PM - out.str().length(), ' ');

	stat(d.path().filename().c_str(), &fileStat);
	auto pw = getpwuid(fileStat.st_uid);
	auto gp = getgrgid(fileStat.st_gid);
	string time = ctime(&fileStat.st_mtime);
	time.pop_back();

	out << pw->pw_name << string(ALIGN_USR - out.str().length(), ' ')
		<< gp->gr_name << string(ALIGN_GR - out.str().length(), ' ')
		<< time;

	cout << (color ? INVERT : "") << out.str() << (color ? REVERT : "") << endl;
}