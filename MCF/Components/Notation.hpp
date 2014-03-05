// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2014. LH_Mouse. All wrongs reserved.

#ifndef __MCF_NOTATION_HPP__
#define __MCF_NOTATION_HPP__

#include "../Core/String.hpp"
#include <utility>
#include <map>
#include <initializer_list>
#include <cwchar>

namespace MCF {

class Notation;

class Package {
	friend class Notation;
private:
	struct xKey {
		const wchar_t *m_pwchBegin;
		std::size_t m_uLength;
		UTF16String m_wcsPermanent;

		xKey(UTF16String &&wcsContents) noexcept {
			m_wcsPermanent = std::move(wcsContents);
			m_pwchBegin = m_wcsPermanent.GetCStr();
			m_uLength = m_wcsPermanent.GetLength();
		}
		xKey(const wchar_t *pwchBegin, std::size_t uLength) noexcept {
			m_pwchBegin = pwchBegin;
			m_uLength = uLength;
		}

		xKey(const xKey &rhs){
			*this = rhs;
		}
		xKey(xKey &&rhs) noexcept {
			*this = std::move(rhs);
		}

		xKey &operator=(const xKey &rhs){
			m_wcsPermanent.Assign(rhs.m_pwchBegin, rhs.m_uLength);
			m_pwchBegin = m_wcsPermanent.GetCStr();
			m_uLength = rhs.m_uLength;
			return *this;
		}
		xKey &operator=(xKey &&rhs) noexcept {
			if(rhs.m_wcsPermanent.IsEmpty()){
				m_wcsPermanent.Assign(rhs.m_pwchBegin, rhs.m_uLength);
			} else {
				m_wcsPermanent = std::move(rhs.m_wcsPermanent);
			}
			m_pwchBegin = m_wcsPermanent.GetCStr();
			m_uLength = rhs.m_uLength;
			return *this;
		}

		bool operator<(const xKey &rhs) const noexcept {
			if(m_uLength != rhs.m_uLength){
				return m_uLength < rhs.m_uLength;
			}
			return std::wmemcmp(m_pwchBegin, rhs.m_pwchBegin, m_uLength) < 0;
		}
	};
private:
	std::map<xKey, Package> xm_mapPackages;
	std::map<xKey, UTF16String> xm_mapValues;
private:
	Package() = default;
public:
	const Package *GetPackage(const wchar_t *pwszName) const noexcept {
		const auto it = xm_mapPackages.find(xKey(pwszName, std::wcslen(pwszName)));
		return (it == xm_mapPackages.end()) ? nullptr : &(it->second);
	}
	Package *GetPackage(const wchar_t *pwszName) noexcept {
		const auto it = xm_mapPackages.find(xKey(pwszName, std::wcslen(pwszName)));
		return (it == xm_mapPackages.end()) ? nullptr : &(it->second);
	}
	Package *CreatePackage(const wchar_t *pwszName){
		xKey Key(pwszName, std::wcslen(pwszName));
		const auto itHint = xm_mapPackages.upper_bound(Key);
		return &(xm_mapPackages.emplace_hint(itHint, std::move(Key), Package())->second);
	}

	const UTF16String *GetValue(const wchar_t *pwszName) const noexcept {
		const auto it = xm_mapValues.find(xKey(pwszName, std::wcslen(pwszName)));
		return (it == xm_mapValues.end()) ? nullptr : &(it->second);
	}
	UTF16String *GetValue(const wchar_t *pwszName) noexcept {
		const auto it = xm_mapValues.find(xKey(pwszName, std::wcslen(pwszName)));
		return (it == xm_mapValues.end()) ? nullptr : &(it->second);
	}
	UTF16String *CreateValue(const wchar_t *pwszName){
		xKey Key(pwszName, std::wcslen(pwszName));
		const auto itHint = xm_mapValues.upper_bound(Key);
		return &(xm_mapValues.emplace_hint(itHint, std::move(Key), UTF16String())->second);
	}

	template<class PATH_SEGMENT_ITER>
	const Package *GetPackage(PATH_SEGMENT_ITER itSegBegin, PATH_SEGMENT_ITER itSegEnd) const noexcept {
		auto ppkgCur = this;
		for(auto it = itSegBegin; it != itSegEnd; ++it){
			if(!(ppkgCur = ppkgCur->GetPackage(*it))){
				return nullptr;
			}
		}
		return ppkgCur;
	}
	template<class PATH_SEGMENT_ITER>
	Package *GetPackage(PATH_SEGMENT_ITER itSegBegin, PATH_SEGMENT_ITER itSegEnd) noexcept {
		auto ppkgCur = this;
		for(auto it = itSegBegin; it != itSegEnd; ++it){
			if(!(ppkgCur = ppkgCur->GetPackage(*it))){
				return nullptr;
			}
		}
		return ppkgCur;
	}
	template<class PATH_SEGMENT_ITER>
	Package *CreatePackage(PATH_SEGMENT_ITER itSegBegin, PATH_SEGMENT_ITER itSegEnd){
		auto ppkgCur = this;
		for(auto it = itSegBegin; it != itSegEnd; ++it){
			ppkgCur = ppkgCur->CreatePackage(*it);
		}
		return ppkgCur;
	}

	template<class PATH_SEGMENT_ITER>
	const UTF16String *GetValue(PATH_SEGMENT_ITER itSegBegin, PATH_SEGMENT_ITER itSegEnd) const noexcept {
		if(itSegBegin == itSegEnd){
			return nullptr;
		}
		auto itName = itSegEnd;
		--itName;
		const auto ppkgParent = GetPackage(itSegBegin, itName);
		if(ppkgParent){
			return nullptr;
		}
		return ppkgParent->GetValue(*itName);
	}
	template<class PATH_SEGMENT_ITER>
	UTF16String *GetValue(PATH_SEGMENT_ITER itSegBegin, PATH_SEGMENT_ITER itSegEnd) noexcept {
		if(itSegBegin == itSegEnd){
			return nullptr;
		}
		auto itName = itSegEnd;
		--itName;
		const auto ppkgParent = GetPackage(itSegBegin, itName);
		if(ppkgParent){
			return nullptr;
		}
		return ppkgParent->GetValue(*itName);
	}
	template<class PATH_SEGMENT_ITER>
	UTF16String *CreateValue(PATH_SEGMENT_ITER itSegBegin, PATH_SEGMENT_ITER itSegEnd){
		if(itSegBegin == itSegEnd){
			return nullptr;
		}
		auto itName = itSegEnd;
		--itName;
		const auto ppkgParent = CreatePackage(itSegBegin, itName);
		return ppkgParent->CreateValue(*itName);
	}

	const Package *GetPackage(std::initializer_list<const wchar_t *> ilPath) const noexcept {
		return GetPackage(ilPath.begin(), ilPath.end());
	}
	Package *GetPackage(std::initializer_list<const wchar_t *> ilPath) noexcept {
		return GetPackage(ilPath.begin(), ilPath.end());
	}
	Package *CreatePackage(std::initializer_list<const wchar_t *> ilPath){
		return CreatePackage(ilPath.begin(), ilPath.end());
	}

	const UTF16String *GetValue(std::initializer_list<const wchar_t *> ilPath) const noexcept {
		return GetValue(ilPath.begin(), ilPath.end());
	}
	UTF16String *GetValue(std::initializer_list<const wchar_t *> ilPath) noexcept {
		return GetValue(ilPath.begin(), ilPath.end());
	}
	UTF16String *CreateValue(std::initializer_list<const wchar_t *> ilPath){
		return CreateValue(ilPath.begin(), ilPath.end());
	}

	void Clear() noexcept {
		xm_mapPackages.clear();
		xm_mapValues.clear();
	}
};

class Notation : public Package {
public:
	typedef enum {
		ERR_NONE,
		ERR_NO_VALUE_NAME,
		ERR_NO_PACKAGE_NAME,
		ERR_UNEXCEPTED_PACKAGE_CLOSE,
		ERR_EQU_EXPECTED,
		ERR_UNCLOSED_PACKAGE,
		ERR_ESCAPE_AT_EOF
	} ERROR_TYPE;
private:
	static void xEscapeAndAppend(UTF16String &wcsAppendTo, const wchar_t *pwchBegin, std::size_t uLength);
	static UTF16String xUnescapeAndConstruct(const wchar_t *pwchBegin, std::size_t uLength);

	static void xExportPackageRecur(UTF16String &wcsAppendTo, const Package &pkgWhich, UTF16String &wcsPrefix, const wchar_t *pwchIndent, std::size_t uIndentLen);
public:
	Notation();
	explicit Notation(const wchar_t *pwszText);
	Notation(const wchar_t *pwchText, std::size_t uLen);
public:
	std::pair<ERROR_TYPE, const wchar_t *> Parse(const wchar_t *pwszText);
	std::pair<ERROR_TYPE, const wchar_t *> Parse(const wchar_t *pwchText, std::size_t uLen);
	UTF16String Export(const wchar_t *pwchIndent = L"\t") const;
};

}

#endif
