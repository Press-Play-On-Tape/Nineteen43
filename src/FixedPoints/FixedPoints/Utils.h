#pragma once

#include "UFixedPoint.h"
#include "FixedPoint.h"

//
// Declaration
//

//
// These functions have a 'Fixed' suffix because
// Arduino defined 'floor', 'ceil', 'round' etc as macros,
// causing an irritating naming conflict.
//

BEGIN_FIXED_POINTS
template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> floorFixed(const UFixedPoint<Integer, Fraction> & value);

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> floorFixed(const FixedPoint<Integer, Fraction> & value);

template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> ceilFixed(const UFixedPoint<Integer, Fraction> & value);

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> ceilFixed(const FixedPoint<Integer, Fraction> & value);

template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> roundFixed(const UFixedPoint<Integer, Fraction> & value);

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> roundFixed(const FixedPoint<Integer, Fraction> & value);

template< unsigned Integer, unsigned Fraction >
constexpr bool signbitFixed(const FixedPoint<Integer, Fraction> & value);
END_FIXED_POINTS

//
// Definition
//

BEGIN_FIXED_POINTS
template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> floorFixed(const UFixedPoint<Integer, Fraction> & value)
{
	using OutputType = UFixedPoint<Integer, Fraction>;
	using InternalType = typename OutputType::InternalType;
	return OutputType::fromInternal(static_cast<InternalType>(value.getInternal() & ~Details::IdentityMask<Fraction>::Value));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> floorFixed(const FixedPoint<Integer, Fraction> & value)
{
	using OutputType = FixedPoint<Integer, Fraction>;
	using InternalType = typename OutputType::InternalType;
	return OutputType::fromInternal(static_cast<InternalType>(value.getInternal() & ~Details::IdentityMask<Fraction>::Value));
}

template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> ceilFixed(const UFixedPoint<Integer, Fraction> & value)
{	
	return UFixedPoint<Integer, Fraction>((value.getFraction() == 0) ? value.getInteger() : (value.getInteger() + 1), 0);
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> ceilFixed(const FixedPoint<Integer, Fraction> & value)
{
	return FixedPoint<Integer, Fraction>((value.getFraction() == 0) ? value.getInteger() : (value.getInteger() + 1), 0);
}

template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> roundFixed(const UFixedPoint<Integer, Fraction> & value)
{
	return (value.getInternal() & Details::MsbMask<Fraction>::Value) != 0 ? ceilFixed(value) : floorFixed(value);
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> roundFixed(const FixedPoint<Integer, Fraction> & value)
{
	return
		(
			(value.getInternal() & Details::MsbMask<Fraction>::Value) != 0 &&
			(signbitFixed(value) ? (value.getInternal() & Details::IdentityMask<Fraction - 1>::Value) != 0 : true)
		)
		? ceilFixed(value)
		: floorFixed(value);
}

template< unsigned Integer, unsigned Fraction >
constexpr bool signbitFixed(const FixedPoint<Integer, Fraction> & value)
{
	return (value.getInternal() & Details::MsbMask<FixedPoint<Integer, Fraction>::InternalSize>::Value) != 0;
}

// Unsigned Random
template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> randomUFixedPoint(void)
{
	using InternalType = typename UFixedPoint<Integer, Fraction>::InternalType;
	return UFixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(random()));
}

template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> randomUFixedPoint(const UFixedPoint<Integer, Fraction> & exclusiveUpperBound)
{
	using InternalType = typename UFixedPoint<Integer, Fraction>::InternalType;
	return UFixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(random(exclusiveUpperBound.getInternal())));
}

template< unsigned Integer, unsigned Fraction >
constexpr UFixedPoint<Integer, Fraction> randomUFixedPoint(const UFixedPoint<Integer, Fraction> & inclusiveLowerBound, const UFixedPoint<Integer, Fraction> & exclusiveUpperBound)
{
	using InternalType = typename UFixedPoint<Integer, Fraction>::InternalType;
	return UFixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(random(inclusiveLowerBound.getInternal(), exclusiveUpperBound.getInternal())));
}

// Signed Random
template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> randomFixedPoint(void)
{
	using InternalType = typename FixedPoint<Integer, Fraction>::InternalType;
	return FixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(random()));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> randomFixedPoint(const FixedPoint<Integer, Fraction> & exclusiveUpperBound)
{
	using InternalType = typename FixedPoint<Integer, Fraction>::InternalType;
	return FixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(random(exclusiveUpperBound.getInternal())));
}

template< unsigned Integer, unsigned Fraction >
constexpr FixedPoint<Integer, Fraction> randomFixedPoint(const FixedPoint<Integer, Fraction> & inclusiveLowerBound, const FixedPoint<Integer, Fraction> & exclusiveUpperBound)
{
	using InternalType = typename FixedPoint<Integer, Fraction>::InternalType;
	return FixedPoint<Integer, Fraction>::fromInternal(static_cast<InternalType>(random(inclusiveLowerBound.getInternal(), exclusiveUpperBound.getInternal())));
}
END_FIXED_POINTS
