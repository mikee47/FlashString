/****
 * Object.hpp - Definitions and macros common to all object types
 *
 * Copyright 2019 mikee47 <mike@sillyhouse.net>
 *
 * This file is part of the FlashString Library
 *
 * This library is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, version 3 or later.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this library.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "Utility.hpp"
#include "ObjectBase.hpp"
#include "ObjectIterator.hpp"

/**
 * @defgroup FlashString FlashString Library
 * @ingroup flash
 * @defgroup fstr_object Objects
 * @ingroup FlashString
 * @{
 */

/**
 * @brief Declare a global Object reference
 * @param name
 * @param ObjectType
 */
#define DECLARE_FSTR_OBJECT(name, ObjectType) extern const ObjectType& name;

/**
 * @name Internal macros
 * @{
 */
#define DEFINE_FSTR_OBJREF(name, object) const decltype(object)& name PROGMEM = object;
#define DEFINE_FSTR_REF(name) DEFINE_FSTR_OBJREF(name, FSTR_DATA_NAME(name).object)
#define DEFINE_FSTR_REF_LOCAL(name) static constexpr DEFINE_FSTR_REF(name)
/** @} */

/**
 * @brief Provide internal name for generated flash string structures
 */
#define FSTR_DATA_NAME(name) __fstr__##name

/**
 * @brief Given an Object& reference, return a pointer to the actual object
 * @param objref
 * @note  This macro was provided for use with old compilers (e.g. GCC 4.8.5) but is no longer required.
 */
#define FSTR_PTR(objref) (&objref)

/**
 * @brief Check structure is POD-compliant and correctly aligned
 */
#define FSTR_CHECK_STRUCT(name)                                                                                        \
	static_assert(std::is_standard_layout<decltype(name)>::value, "FSTR structure not Standard Layout");               \
	static_assert(offsetof(decltype(name), data) == sizeof(uint32_t), "FSTR structure alignment error");

/**
 * @brief Import an object from an external file with reference
 * @param name Name for the object
 * @param ObjectType Object type for reference
 * @param file Absolute path to the file containing the content
 * @see See also `IMPORT_FSTR_DATA`
 * @note Can only be used at file scope
 */
#define IMPORT_FSTR_OBJECT(name, ObjectType, file)                                                                     \
	IMPORT_FSTR_DATA(FSTR_DATA_NAME(name), file)                                                                       \
	extern "C" __attribute__((visibility("hidden"))) const ObjectType FSTR_DATA_NAME(name);                            \
	DEFINE_FSTR_OBJREF(name, FSTR_DATA_NAME(name))

/**
 * @brief Like IMPORT_FSTR_OBJECT except reference is declared static constexpr
 */
#define IMPORT_FSTR_OBJECT_LOCAL(name, ObjectType, file)                                                               \
	IMPORT_FSTR_DATA(FSTR_DATA_NAME(name), file)                                                                       \
	extern "C" __attribute__((visibility("hidden"))) const ObjectType FSTR_DATA_NAME(name);                            \
	static constexpr DEFINE_FSTR_OBJREF(name, FSTR_DATA_NAME(name))

namespace FSTR
{
/**
 * @brief Base class template for all types
 * @tparam ObjectType The object type actually being instantiated
 * @tparam ElementType
 * @see https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */
template <class ObjectType, typename ElementType> class Object : public ObjectBase
{
public:
	using DataPtrType = const ElementType*;
	using Iterator = ObjectIterator<ObjectType, ElementType>;

	Object(const Object&) = delete;
	Object(const Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(const Object&&) = delete;

	Iterator begin() const
	{
		return Iterator(as<ObjectType>(), 0);
	}

	Iterator end() const
	{
		return Iterator(as<ObjectType>(), length());
	}

	/**
	 * @brief Return an empty object which evaluates to null
	 */
	static constexpr const ObjectType& empty()
	{
		return empty_.as<const ObjectType>();
	}

	/**
	 * @brief Get the length of the content in elements
	 */
	FSTR_INLINE constexpr const size_t length() const
	{
		return ObjectBase::length() / sizeof(ElementType);
	}

	template <typename ValueType> int indexOf(const ValueType& value) const
	{
		auto& self = as<ObjectType>();
		auto dataptr = self.data();
		auto len = self.length();
		for(unsigned i = 0; i < len; ++i) {
			if(self.unsafeValueAt(dataptr, i) == value) {
				return int(i);
			}
		}

		return -1;
	}

	FSTR_INLINE ElementType valueAt(unsigned index) const
	{
		return (index < length()) ? unsafeValueAt(data(), index) : ElementType{};
	}

	/**
	 * @brief Array operator[]
	 */
	FSTR_INLINE ElementType operator[](unsigned index) const
	{
		return valueAt(index);
	}

	FSTR_INLINE size_t elementSize() const
	{
		return sizeof(ElementType);
	}

	FSTR_INLINE DataPtrType data() const
	{
		return reinterpret_cast<DataPtrType>(ObjectBase::data());
	}

	/**
	 * @brief Read content into RAM
	 * @param index First element to read
	 * @param buffer Where to store data
	 * @param count How many elements to read
	 * @retval size_t Number of elements actually read
	 */
	size_t read(size_t index, ElementType* buffer, size_t count) const
	{
		auto offset = index * sizeof(ElementType);
		count *= sizeof(ElementType);
		return ObjectBase::read(offset, buffer, count) / sizeof(ElementType);
	}

	/**
	 * @brief Read content into RAM,using `flashmem_read()`
	 * @param index First element to read
	 * @param buffer Where to store data
	 * @param count How many elements to read
	 * @retval size_t Number of elements actually read
	 */
	size_t readFlash(size_t index, ElementType* buffer, size_t count) const
	{
		auto offset = index * sizeof(ElementType);
		count *= sizeof(ElementType);
		return ObjectBase::readFlash(offset, buffer, count) / sizeof(ElementType);
	}

	FSTR_INLINE ElementType unsafeValueAt(const DataPtrType dataptr, unsigned index) const
	{
		return readValue(dataptr + index);
	}
};

} // namespace FSTR

/** @} */
