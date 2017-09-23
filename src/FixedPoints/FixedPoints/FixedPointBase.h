#pragma once

#include "Details.h"

BEGIN_FIXED_POINTS
namespace Details
{
	template< unsigned Integer, unsigned Fraction >
	class FixedPointBase
	{
	public:
		using IntegerType = Details::LeastInt<Integer + 1>;
		using FractionType = Details::LeastUInt<Fraction>;
		using InternalType = Details::LeastInt<(Integer + 1) + Fraction>;

		constexpr const static unsigned long long Scale = 1ULL << Fraction;

	protected:
		class RawType
		{
		private:
			const InternalType value;

		public:
			constexpr inline explicit RawType(const InternalType & value) : value(value) {}
			constexpr inline explicit operator InternalType(void) const { return this->value; }
		};

	protected:
		InternalType value;

		constexpr FixedPointBase(void) : value(0) {}
		constexpr FixedPointBase(const RawType & value) : value(static_cast<InternalType>(value)) {}
		
	public:
		constexpr FixedPointBase(const IntegerLiteral & value)
			: value(static_cast<InternalType>(static_cast<LargerType<IntegerLiteral, InternalType> >(value) <<  Fraction)) {}
			
		constexpr FixedPointBase(const IntegerLiteralU & value)
			: value(static_cast<InternalType>(static_cast<LargerType<IntegerLiteralU, InternalType> >(value) <<  Fraction)) {}
			
		constexpr FixedPointBase(const IntegerLiteralL & value)
			: value(static_cast<InternalType>(static_cast<LargerType<IntegerLiteralL, InternalType> >(value) <<  Fraction)) {}
			
		constexpr FixedPointBase(const IntegerLiteralUL & value)
			: value(static_cast<InternalType>(static_cast<LargerType<IntegerLiteralUL, InternalType>>(value) <<  Fraction)) {}
			
		constexpr FixedPointBase(const IntegerLiteralLL & value)
			: value(static_cast<InternalType>(static_cast<LargerType<IntegerLiteralLL, InternalType>>(value) <<  Fraction)) {}
			
		constexpr FixedPointBase(const IntegerLiteralULL & value)
			: value(static_cast<InternalType>(static_cast<LargerType<IntegerLiteralULL, InternalType> >(value) <<  Fraction)) {}
			
		constexpr FixedPointBase(const DecimalLiteral & value)
			: value(static_cast<InternalType>(value * Scale)) {}
			
		constexpr FixedPointBase(const DecimalLiteralF & value)
			: value(static_cast<InternalType>(value * Scale)) {}
			
		constexpr FixedPointBase(const DecimalLiteralL & value)
			: value(static_cast<InternalType>(value * Scale)) {}
	};
}
END_FIXED_POINTS