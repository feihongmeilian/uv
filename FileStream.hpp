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
		inline explicit FileStream(uv::Loop &loop);

		inline void		reqCleanup();
		inline int		close(uv_file file, std::function<void(uv::FileStream &)> handler);
		inline int		open(const std::string &path, int flags, int mode, std::function<void(uv::FileStream &)> handler);
		inline int		read(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, std::function<void(uv::FileStream &)> handler);
		inline int		write(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, std::function<void(uv::FileStream &)> handler);
		inline int		unlink(const std::string &path, std::function<void(uv::FileStream &)> handler);
		inline int		mkdir(const std::string &path, int mode, std::function<void(uv::FileStream &)> handler);
		inline int		mkdtemp(const std::string &tpl, std::function<void(uv::FileStream &)> handler);
		inline int		rmdir(const std::string &path, std::function<void(uv::FileStream &)> handler);
		inline int		scandir(const std::string &path, int flags, std::function<void(uv::FileStream &)> handler);
		inline int		scandirNext(uv_dirent_t* ent);
		inline int		stat(const std::string &path, std::function<void(uv::FileStream &)> handler);
		inline int		fstat(uv_file file,std::function<void(uv::FileStream &)> handler);
		inline int		rename(const std::string &path, const std::string &new_path, std::function<void(uv::FileStream &)> handler);
		inline int		fsync(uv_file file, std::function<void(uv::FileStream &)> handler);
		inline int		fdatasync(uv_file file, std::function<void(uv::FileStream &)> handler);
		inline int		ftruncate(uv_file file, int64_t offset, std::function<void(uv::FileStream &)> handler);
		inline int		sendfile(uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length,std::function<void(uv::FileStream &)> handler);
		inline int		access(const std::string &path, int mode, std::function<void(uv::FileStream &)> handler);
		inline int		chmod(const std::string &path, int mode, std::function<void(uv::FileStream &)> handler);
		inline int		utime(const std::string &path, double atime, double mtime, std::function<void(uv::FileStream &)> handler);
		inline int		futime(uv_file file, double atime, double mtime, std::function<void(uv::FileStream &)> handler);
		inline int		lstat(const std::string &path, std::function<void(uv::FileStream &)> handler);
		inline int		link(const std::string &path, const std::string &newPath, std::function<void(uv::FileStream &)> handler);
		inline int		symlink(const std::string &path, const std::string &newPath, int flags, std::function<void(uv::FileStream &)> handler);
		inline int		readlink(const std::string &path, std::function<void(uv::FileStream &)> handler);
		inline int		realpath(const std::string &path, std::function<void(uv::FileStream &)> handler);
		inline int		fchmod(uv_file file, int mode, std::function<void(uv::FileStream &)> handler);
		inline int		chown(const std::string &path, uv_uid_t uid, uv_gid_t gid, std::function<void(uv::FileStream &)> handler);
		inline int		fchown(uv_file file, uv_uid_t uid, uv_gid_t gid, std::function<void(uv::FileStream &)> handler);
	private:
		uv::Loop			&m_loop;
		std::function<void(uv::FileStream &)>	m_callbackHandler = [](uv::FileStream &) {};
	};





	FileStream::FileStream(uv::Loop &loop)
		: m_loop(loop)
	{
		m_handle.data = this;
	}

	void FileStream::reqCleanup()
	{
		uv_fs_req_cleanup(&m_handle);
	}

	int FileStream::close(uv_file file, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_close(&m_loop.m_loop, &m_handle, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::open(const std::string &path, int flags, int mode, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_open(&m_loop.m_loop, &m_handle, path.c_str(), flags, mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::read(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_read(&m_loop.m_loop, &m_handle, file, bufs, nbufs, offset, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::write(uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_write(&m_loop.m_loop, &m_handle, file, bufs, nbufs, offset, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::unlink(const std::string &path, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_unlink(&m_loop.m_loop, &m_handle, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::mkdir(const std::string &path, int mode, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_mkdir(&m_loop.m_loop, &m_handle, path, .c_str(), mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::mkdtemp(const std::string &tpl, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_mkdtemp(&m_loop.m_loop, &m_handle, tpl.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::rmdir(const std::string &path, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_rmdir(&m_loop.m_loop, &m_handle, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::scandir(const std::string &path, int flags, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_scandir(&m_loop.m_loop, &m_handle, path.c_str(), flags, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::scandirNext(uv_dirent_t *ent)
	{
		return uv_fs_scandir_next(&m_handle, ent);
	}

	int FileStream::stat(const std::string &path, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_stat(&m_loop.m_loop, &m_handle, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::fstat(uv_file file, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_fstat(&m_loop.m_loop, &m_handle, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::rename(const std::string &path, const std::string &new_path, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_rename(&m_loop.m_loop, &m_handle, path.c_str(), new_path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::fsync(uv_file file, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_fsync(&m_loop.m_loop, &m_handle, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::fdatasync(uv_file file, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_fdatasync(&m_loop.m_loop, &m_handle, file, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::ftruncate(uv_file file, int64_t offset, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_ftruncate(&m_loop.m_loop, &m_handle, file, offset, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::sendfile(uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_sendfile(&m_loop.m_loop, &m_handle, out_fd, in_fd, in_offset, length, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::access(const std::string &path, int mode, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_access(&m_loop.m_loop, &m_handle, path.c_str(), mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::chmod(const std::string &path, int mode, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_chmod(&m_loop.m_loop, &m_handle, path.c_str(), mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::utime(const std::string &path, double atime, double mtime, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_utime(&m_loop.m_loop, &m_handle, path.c_str(), atime, mtime, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::futime(uv_file file, double atime, double mtime, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_futime(&m_loop.m_loop, &m_handle, file, atime, mtime, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::lstat(const std::string &path, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_lstat(&m_loop.m_loop, &m_handle, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::link(const std::string &path, const std::string &newPath, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_link(&m_loop.m_loop, &m_handle, path.c_str(), newPath.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::symlink(const std::string &path, const std::string &newPath, int flags, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_symlink(&m_loop.m_loop, &m_handle, path.c_str(), newPath.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::readlink(const std::string &path, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_readlink(&m_loop.m_loop, &m_handle, path.c_str(), [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::realpath(const std::string &path, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_realpath(&m_loop.m_loop, &m_handle, path.c_str(),[](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::fchmod(uv_file file, int mode, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_fchmod(&m_loop.m_loop, &m_handle, file, mode, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::chown(const std::string &path, uv_uid_t uid, uv_gid_t gid, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_chown(&m_loop.m_loop, &m_handle, path.c_str(), uid, gid, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

	int FileStream::fchown(uv_file file, uv_uid_t uid, uv_gid_t gid, std::function<void(uv::FileStream &)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_fchown(&m_loop.m_loop, &m_handle, file, uid, gid, [](uv_fs_t *req) {
			auto &fs = *reinterpret_cast<uv::FileStream *>(req->data);
			fs.m_callbackHandler(fs);
		});
	}

}


#endif
