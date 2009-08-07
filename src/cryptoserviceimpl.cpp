#include "cryptoserviceimpl.h"
#include <QCryptographicHash>

namespace Core
{
	CryptoServiceImpl::CryptoServiceImpl()
	{
		// We use AES-256, so we need vector with length 32
		m_iv = QByteArray::fromHex("c898e1c1771eb0bc4dc846d5edba0005"
								   "a54d2bb6f0d24fbfbb3c58a977edc50f");
		m_cipher_enc = 0;
		m_cipher_dec = 0;
	}

	QByteArray CryptoServiceImpl::cryptImpl(const QByteArray &value) const
	{
		if(!m_cipher_enc)
			return value;
		QByteArray result;
		for(int i = 0x0; i < value.size(); i += 0xf)
		{
			m_cipher_enc->clear();
			QCA::SecureArray arg = value.mid(i, 0xf);
			m_cipher_enc->update(arg);
			if(!m_cipher_enc->ok())
				return result;
			result += m_cipher_enc->final().toByteArray();
		}
		return result;
	}

	QByteArray CryptoServiceImpl::decryptImpl(const QByteArray &value) const
	{
		if(!m_cipher_dec)
			return value;
		QByteArray result;
		for(int i = 0x0; i < value.size(); i += 0x10)
		{
			m_cipher_dec->clear();
			QCA::SecureArray arg = value.mid(i, 0x10);
			m_cipher_dec->update(arg);
			if(!m_cipher_dec->ok())
				return result;
			result += m_cipher_dec->final().toByteArray();
		}
		return result;
	}

	void CryptoServiceImpl::setPassword(const QString &password)
	{
		QByteArray key = QCA::Hash("sha256").hash(password.toUtf8()).toByteArray();
		m_key = key;
		m_cipher_enc = new QCA::Cipher(QString("aes256"),QCA::Cipher::CBC,
									   QCA::Cipher::DefaultPadding,
									   QCA::Encode, m_key, m_iv);
		m_cipher_dec = new QCA::Cipher(QString("aes256"),QCA::Cipher::CBC,
									   QCA::Cipher::DefaultPadding,
									   QCA::Decode, m_key, m_iv);
	}
}
