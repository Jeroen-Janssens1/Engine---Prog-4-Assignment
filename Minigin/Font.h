#pragma once

struct _TTF_Font;
	/**
	 * Simple RAII wrapper for an _TTF_Font
	 */
	class Font
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		Font(const Font&) = delete;
		Font(Font&&) = delete;
		Font& operator= (const Font&) = delete;
		Font& operator= (const Font&&) = delete;

		void GetFileName(std::string& fileName) const;
		void SetSize(unsigned int size);
		unsigned int GetSize() const { return m_Size; }
	private:
		_TTF_Font* m_pFont;
		unsigned int m_Size;
		const std::string m_FileName;
	};
