#ifndef UV_FILE_STREAM_HPP
#define UV_FILE_STREAM_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Req.hpp"
#include "Loop.hpp"

namespace uv
{
	class FileStream : public Req<uv_fs_t>
	{
	public:
		explicit	FileStream(uv::Loop &loop);

		void        reqCleanup();
		int			close(uv_file file, const std::function<void()> &handler);
		int			open(const std::string &path, int flags, int mode, const std::function<void()> &handler);
		int			read(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, const std::function<void()> &handler);
		int			write(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, const std::function<void()> &handler);
		int			unlink(const std::string &path, const std::function<void()> &handler);
		int			mkdir(const std::string &path, int mode, const std::function<void()> &handler);
		int			mkdtemp(const std::string &tpl, const std::function<void()> &handler);
		int			rmdir(const std::string &path, const std::function<void()> &handler);
		int			scandir(const std::string &path, int flags, const std::function<void()> &handler);
		int			scandirNext(uv_dirent_t* ent);
		int			stat(const std::string &path, const std::function<void()> &handler);
		int			fstat(uv_file file,const std::function<void()> &handler);
		int			rename(const std::string &path, const std::string &new_path, const std::function<void()> &handler);
		int			fsync(uv_file file, const std::function<void()> &handler);
		int			fdatasync(uv_file file, const std::function<void()> &handler);
		int			ftruncate(uv_file file, int64_t offset, const std::function<void()> &handler);
		int			sendfile(uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, const std::function<void()> &handler);
		int			access(const std::string &path, int mode, const std::function<void()> &handler);
		int			chmod(const std::string &path, int mode, const std::function<void()> &handler);
		int			utime(const std::string &path, double atime, double mtime, const std::function<void()> &handler);
		int			futime(uv_file file, double atime, double mtime, const std::function<void()> &handler);
		int			lstat(const std::string &path, const std::function<void()> &handler);
		int			link(const std::string &path, const std::string &newPath, const std::function<void()> &handler);
		int			symlink(const std::string &path, const std::string &newPath, int flags, const std::function<void()> &handler);
		int			readlink(const std::string &path, const std::function<void()> &handler);
		int			realpath(const std::string &path, const std::function<void()> &handler);
		int			fchmod(uv_file file, int mode, const std::function<void()> &handler);
		int			chown(const std::string &path, uv_uid_t uid, uv_gid_t gid, const std::function<void()> &handler);
		int			fchown(uv_file file, uv_uid_t uid, uv_gid_t gid, const std::function<void()> &handler);
	private:
		uv::Loop	&loop_;
		std::function<void()>	callbackHandler_ = []() {};
	};





	inline FileStream::FileStream(uv::Loop &loop)
		: loop_(loop)
	{
		handle_.data = this;
	}

	inline void FileStream::reqCleanup()
	{
		uv_fs_req_cleanup(&handle_);
	}

	inline int FileStream::close(uv_file file, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_close(loop_.value(), &handle_, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::open(const std::string &path, int flags, int mode, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_open(loop_.value(), &handle_, path.c_str(), flags, mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::read(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_read(loop_.value(), &handle_, file, bufs, nbufs, offset, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::write(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_write(loop_.value(), &handle_, file, bufs, nbufs, offset, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::unlink(const std::string &path, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_unlink(loop_.value(), &handle_, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::mkdir(const std::string &path, int mode, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_mkdir(loop_.value(), &handle_, path.c_str(), mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::mkdtemp(const std::string &tpl, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_mkdtemp(loop_.value(), &handle_, tpl.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::rmdir(const std::string &path, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_rmdir(loop_.value(), &handle_, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::scandir(const std::string &path, int flags, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_scandir(loop_.value(), &handle_, path.c_str(), flags, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::scandirNext(uv_dirent_t *ent)
	{
		return uv_fs_scandir_next(&handle_, ent);
	}

	inline int FileStream::stat(const std::string &path, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_stat(loop_.value(), &handle_, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::fstat(uv_file file, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_fstat(loop_.value(), &handle_, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::rename(const std::string &path, const std::string &new_path, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_rename(loop_.value(), &handle_, path.c_str(), new_path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::fsync(uv_file file, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_fsync(loop_.value(), &handle_, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::fdatasync(uv_file file, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_fdatasync(loop_.value(), &handle_, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::ftruncate(uv_file file, int64_t offset, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_ftruncate(loop_.value(), &handle_, file, offset, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::sendfile(uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_sendfile(loop_.value(), &handle_, out_fd, in_fd, in_offset, length, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::access(const std::string &path, int mode, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_access(loop_.value(), &handle_, path.c_str(), mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::chmod(const std::string &path, int mode, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_chmod(loop_.value(), &handle_, path.c_str(), mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::utime(const std::string &path, double atime, double mtime, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_utime(loop_.value(), &handle_, path.c_str(), atime, mtime, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::futime(uv_file file, double atime, double mtime, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_futime(loop_.value(), &handle_, file, atime, mtime, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::lstat(const std::string &path, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_lstat(loop_.value(), &handle_, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::link(const std::string &path, const std::string &newPath, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_link(loop_.value(), &handle_, path.c_str(), newPath.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::symlink(const std::string &path, const std::string &newPath, int flags, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_symlink(loop_.value(), &handle_, path.c_str(), newPath.c_str(), flags, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::readlink(const std::string &path, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_readlink(loop_.value(), &handle_, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::realpath(const std::string &path, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_realpath(loop_.value(), &handle_, path.c_str(),[](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::fchmod(uv_file file, int mode, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_fchmod(loop_.value(), &handle_, file, mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::chown(const std::string &path, uv_uid_t uid, uv_gid_t gid, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_chown(loop_.value(), &handle_, path.c_str(), uid, gid, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

	inline int FileStream::fchown(uv_file file, uv_uid_t uid, uv_gid_t gid, const std::function<void()> &handler)
	{
		callbackHandler_ = handler;
		return uv_fs_fchown(loop_.value(), &handle_, file, uid, gid, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.callbackHandler_();
		});
	}

}


#endif
