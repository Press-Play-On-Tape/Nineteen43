#pragma once

#include <math.h>
#include <stdint.h>

#include "Details.h"

#include "FixedPointBase.h"

//
// Declaration
//

BEGIN_FIXED_POINTS
template< unsigned Integer, unsigned Fraction >
class FixedPoint : Details::FixedPointBase< Integer, Fraction >
{
public:
	static_assert(((Integer + 1) + Fraction) <= Details::BitSize<uint_least64_t>::Value, "Platform does not have type capable of containing.");

public:
	using IntegerType = Details::LeastInt<Integer + 1>;
	using FractionType = Details::LeastUInt<Fraction>;
	using InternalType = Details::LeastInt<(Integer + 1) + Fraction>;

	constexpr const static unsigned IntegerSize = Integer + 1;
	constexpr const static unsigned FractionSize = Fraction;
	constexpr const static unsigned LogicalSize = IntegerSize + FractionSize;
	constexpr const static unsigned InternalSize = Details::BitSize<InternalType>::Value;	
	
	constexpr const static unsigned long long Scale = 1ULL << FractionSize;
	
private:
	using Base = Details::FixedPointBase<Integer, Fraction>;
	using RawType = typename Base::RawType;

public:
	using Base::Base;

	constexpr FixedPoint(void);
	constexpr FixedPoint(const IntegerType & integer);
	constexpr FixedPoint(const IntegerType & integer, const FractionType & fraction);

	constexpr InternalType getInternal(void) const;
	constexpr IntegerType getInteger(void) const;
	constexpr FractionType getFraction(void) const;

	constexpr explicit operator IntegerType(void) const;
	constexpr explicit operator float(void) const;
	constexpr explicit operator double(void) const;

	template< unsigned IntegerOut, unsigned FractionOut >
	constexpr explicit operator FixedPoint<IntegerOut, FractionOut>(void) const;

	constexpr static FixedPoint fromInternal(const InternalType & value);

	constexpr FixedPoint operator -(void) const;
	FixedPoint & operator ++(void);
	FixedPoint & operator --(void);
	FixedPoint & operator +=(const FixedPoint & other);
	FixedPoint & operator -=(const FixedPoint & other);
	FixedPoint & operator *=(const FixedPoint & other);
	FixedPoint & operator /=(const FixedPoint & other);
	
public:
	constexpr const static FixedPoint MinValue = FixedPoint::fromInternal(Details::MsbMask<InternalSize>::Value);
	constexpr const static FixedPoint MaxValue = FixedPoint::fromInternal(~Details::MsbMask<InternalSize>::Value);
};

template< unsigned Integer, unsigned Fraction >
constexpr bool operator ==(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator ==(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right);

template< unsigned Integer, unsigned Fraction >
constexpr bool operator !=(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator !=(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right);

template< unsigned Integer, unsigned Fraction >
constexpr bool operator <(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator <(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right);

template< unsigned Integer, unsigned Fraction >
constexpr bool operator >(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator >(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right);

template< unsigned Integer, unsigned Fraction >
constexpr bool operator <=(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator <=(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right);

template< unsigned Integer, unsigned Fraction >
constexpr bool operator >=(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator >=(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right);

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator +(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr auto operator +(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >;

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator -(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr auto operator -(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >;

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator *(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr auto operator *(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >;

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator /(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
inline constexpr auto operator /(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >;

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer * 2, Fraction * 2> multiply(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right);
END_FIXED_POINTS

//
// Definition
//

BEGIN_FIXED_POINTS
template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction>::FixedPoint(void)
	: Base()
{
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction>::FixedPoint(const IntegerType & integer)
	: Base(RawType(static_cast<InternalType>(integer) << FractionSize))
{
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction>::FixedPoint(const IntegerType & integer, const FractionType & fraction)
	: Base(RawType((static_cast<InternalType>(integer) << FractionSize) | fraction))
{
}

template< unsigned Integer, unsigned Fraction >
constexpr typename FixedPoint<Integer, Fraction>::InternalType FixedPoint<Integer, Fraction>::getInternal(void) const
{
	return this->value;
}

template< unsigned Integer, unsigned Fraction >
constexpr typename FixedPoint<Integer, Fraction>::IntegerType FixedPoint<Integer, Fraction>::getInteger(void) const
{
	return static_cast<IntegerType>(this->value >> FractionSize);
}

template< unsigned Integer, unsigned Fraction >
constexpr typename FixedPoint<Integer, Fraction>::FractionType FixedPoint<Integer, Fraction>::getFraction(void) const
{
	return static_cast<FractionType>(this->value & Details::IdentityMask<FractionSize>::Value);
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction>::operator IntegerType(void) const
{
	return this->getInteger();
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction>::operator float(void) const
{
	return (this->value * (1.0f / Scale));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction>::operator double(void) const
{
	return (this->value * (1.0 / Scale));
}

template< unsigned Integer, unsigned Fraction >
template< unsigned IntegerOut, unsigned FractionOut >
constexpr FixedPoint<Integer, Fraction>::operator FixedPoint<IntegerOut, FractionOut>(void) const
{	
	using OutputType = FixedPoint<IntegerOut, FractionOut>;
	using OutputInternalType = typename OutputType::InternalType;
	using OutputShiftType = Details::LeastInt<Details::BitSize<OutputInternalType>::Value>;
	
	using InputType = FixedPoint<Integer, Fraction>;
	using InputInternalType = typename InputType::InternalType;
	using InputShiftType = Details::LeastInt<Details::BitSize<InputInternalType>::Value>;
	
	return
	(FractionOut > Fraction) ?
		OutputType::fromInternal(static_cast<OutputInternalType>(static_cast<OutputShiftType>(this->value) << ((FractionOut > Fraction) ? (FractionOut - Fraction) : 0))) :
	(Fraction > FractionOut) ?
		OutputType::fromInternal(static_cast<OutputInternalType>(static_cast<InputShiftType>(this->value) >> ((Fraction > FractionOut) ? (Fraction - FractionOut) : 0))) :
		OutputType::fromInternal(this->value);
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> FixedPoint<Integer, Fraction>::fromInternal(const typename FixedPoint<Integer, Fraction>::InternalType & value)
{
	return FixedPoint<Integer, Fraction>(RawType(value));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> FixedPoint<Integer, Fraction>::operator -(void) const
{
	return FixedPoint<Integer, Fraction>::fromInternal(-this->value);
}

template< unsigned Integer, unsigned Fraction >
FixedPoint<Integer, Fraction> & FixedPoint<Integer, Fraction>::operator ++(void)
{
	this->value += (1 << FractionSize);
	return *this;
}

template< unsigned Integer, unsigned Fraction >
FixedPoint<Integer, Fraction> & FixedPoint<Integer, Fraction>::operator --(void)
{
	this->value -= (1 << FractionSize);
	return *this;
}

template< unsigned Integer, unsigned Fraction >
FixedPoint<Integer, Fraction> & FixedPoint<Integer, Fraction>::operator +=(const FixedPoint<Integer, Fraction> & other)
{
	this->value += other.value;
	return *this;
}

template< unsigned Integer, unsigned Fraction >
FixedPoint<Integer, Fraction> & FixedPoint<Integer, Fraction>::operator -=(const FixedPoint<Integer, Fraction> & other)
{
	this->value -= other.value;
	return *this;
}

template< unsigned Integer, unsigned Fraction >
FixedPoint<Integer, Fraction> & FixedPoint<Integer, Fraction>::operator *=(const FixedPoint<Integer, Fraction> & other)
{
	using PrecisionType = typename FixedPoint<Integer * 2, Fraction * 2>::InternalType;
	const PrecisionType temp = (static_cast<PrecisionType>(this->value) * static_cast<PrecisionType>(other.value)) >> Fraction;
	this->value = temp;
	return *this;
}

template< unsigned Integer, unsigned Fraction >
FixedPoint<Integer, Fraction> & FixedPoint<Integer, Fraction>::operator /=(const FixedPoint<Integer, Fraction> & other)
{
	using PrecisionType = typename FixedPoint<Integer * 2, Fraction * 2>::InternalType;
	const PrecisionType temp = (static_cast<PrecisionType>(this->value) << Fraction) / static_cast<PrecisionType>(other.value);
	this->value = temp;
	return *this;
}



template< unsigned Integer, unsigned Fraction >
constexpr bool operator ==(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	return (left.getInternal() == right.getInternal());
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator ==(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator == has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) == static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr bool operator !=(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	return (left.getInternal() != right.getInternal());
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator !=(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator != has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) != static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr bool operator <(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	return (left.getInternal() < right.getInternal());
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator <(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator < has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) < static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr bool operator >(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	return (left.getInternal() > right.getInternal());
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator >(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator > has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) > static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr bool operator <=(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	return (left.getInternal() <= right.getInternal());
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator <=(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator <= has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) <= static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr bool operator >=(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	return (left.getInternal() >= right.getInternal());
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr bool operator >=(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator >= has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) >= static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator +(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	using InternalType = typename FixedPoint<Integer, Fraction>::InternalType;
	return FixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(left.getInternal() + right.getInternal()));
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr auto operator +(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator + has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) + static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator -(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	using InternalType = typename FixedPoint<Integer, Fraction>::InternalType;
	return FixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(left.getInternal() - right.getInternal()));
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr auto operator -(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator - has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) - static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator *(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	return static_cast<FixedPoint<Integer, Fraction>>(multiply(left, right));
}

template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr auto operator *(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator * has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) * static_cast<CompareType>(right));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> operator /(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{
	using InternalType = typename FixedPoint<Integer, Fraction>::InternalType;
	using PrecisionType = typename FixedPoint<Integer * 2, Fraction * 2>::InternalType;
	return FixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>((static_cast<PrecisionType>(left.getInternal()) << Fraction) / right.getInternal()));
}


template< unsigned IntegerLeft, unsigned FractionLeft, unsigned IntegerRight, unsigned FractionRight >
constexpr auto operator /(const FixedPoint<IntegerLeft, FractionLeft> & left, const FixedPoint<IntegerRight, FractionRight> & right)
	-> Details::LargerType< FixedPoint<IntegerLeft, FractionLeft>, FixedPoint<IntegerRight, FractionRight> >
{
	using LeftType = FixedPoint<IntegerLeft, FractionLeft>;
	using RightType = FixedPoint<IntegerRight, FractionRight>;

	static_assert(LeftType::InternalSize != RightType::InternalSize, "operator / has ambiguous result");
	
	using CompareType = Details::LargerType<LeftType, RightType>;
	
	return (static_cast<CompareType>(left) / static_cast<CompareType>(right));
}

#define OPERATOR_LOGIC( type, op ) \
	template< unsigned Integer, unsigned Fraction > \
	constexpr bool operator op (const FixedPoint<Integer, Fraction> & left, const type & right) \
	{ \
		return (left op FixedPoint<Integer, Fraction>(right)); \
	} \
	\
	template< unsigned Integer, unsigned Fraction > \
	constexpr bool operator op (const type & left, const FixedPoint<Integer, Fraction> & right) \
	{ \
		return (FixedPoint<Integer, Fraction>(left) op right); \
	}

#define OPERATOR_ARITHMETIC( type, op ) \
	template< unsigned Integer, unsigned Fraction > \
	constexpr FixedPoint<Integer, Fraction> operator op (const FixedPoint<Integer, Fraction> & left, const type & right) \
	{ \
		return (left op FixedPoint<Integer, Fraction>(right)); \
	} \
	\
	template< unsigned Integer, unsigned Fraction > \
	constexpr FixedPoint<Integer, Fraction> operator op (const type & left, const FixedPoint<Integer, Fraction> & right) \
	{ \
		return (FixedPoint<Integer, Fraction>(left) op right); \
	}
	
#define OPERATORS( opType, op ) \
	OPERATOR_##opType( Details::IntegerLiteral, op ) \
	OPERATOR_##opType( Details::IntegerLiteralU, op ) \
	OPERATOR_##opType( Details::IntegerLiteralL, op ) \
	OPERATOR_##opType( Details::IntegerLiteralUL, op ) \
	OPERATOR_##opType( Details::IntegerLiteralLL, op ) \
	OPERATOR_##opType( Details::IntegerLiteralULL, op ) \
	OPERATOR_##opType( Details::DecimalLiteral, op ) \
	OPERATOR_##opType( Details::DecimalLiteralF, op ) \
	OPERATOR_##opType( Details::DecimalLiteralL, op )

OPERATORS( LOGIC, == )
OPERATORS( LOGIC, != )
OPERATORS( LOGIC, < )
OPERATORS( LOGIC, > )
OPERATORS( LOGIC, <= )
OPERATORS( LOGIC, >= )

OPERATORS( ARITHMETIC , + )
OPERATORS( ARITHMETIC, - )
OPERATORS( ARITHMETIC, * )
OPERATORS( ARITHMETIC, / )

#undef OPERATOR_LOGIC
#undef OPERATOR_ARITHMETIC
#undef OPERATORS

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer * 2, Fraction * 2> multiply(const FixedPoint<Integer, Fraction> & left, const FixedPoint<Integer, Fraction> & right)
{	
	using ResultType = FixedPoint<Integer * 2, Fraction * 2>;
	using InternalType = typename ResultType::InternalType;
	return ResultType::fromInternal(static_cast<InternalType>(static_cast<InternalType>(left.getInternal()) * static_cast<InternalType>(right.getInternal())));
}
END_FIXED_POINTS