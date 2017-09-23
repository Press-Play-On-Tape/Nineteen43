#pragma once

#include <limits.h>
#include <stdint.h>

#if defined(FIXED_POINTS_NAMESPACE)
#define FIXED_POINTS_NAMESPACE FixedPoints
#define BEGIN_FIXED_POINTS namespace FIXED_POINTS_NAMESPACE \
{
#define END_FIXED_POINTS }
#else
#define FIXED_POINTS_NAMESPACE
#define BEGIN_FIXED_POINTS
#define END_FIXED_POINTS
#endif

// Pay no attention to the man behind the curtains

BEGIN_FIXED_POINTS
namespace Details
{
	template< typename T >
	struct BitSize
	{
		BitSize(void) = delete;
		constexpr static const auto Value = sizeof(T) * CHAR_BIT;
	};

	template< typename T, typename U = T >
	constexpr auto Max(const T & left, const U & right) noexcept -> decltype((left > right) ? left : right)
	{
		return (left > right) ? left : right;
	}
	
	template< typename T, T Left, T Right >
	constexpr auto Max(void) noexcept -> decltype((Left > Right) ? Left : Right)
	{
		return (Left > Right) ? Left : Right;
	}

	template< typename T, typename U = T >
	constexpr auto Min(const T & left, const U & right) noexcept -> decltype((left < right) ? left : right)
	{
		return (left < right) ? left : right;
	}
	
	template< typename T, T Left, T Right >
	constexpr auto Min(void) noexcept -> decltype((Left < Right) ? Left : Right)
	{
		return (Left < Right) ? Left : Right;
	}

	template< bool Condition, typename TTrue, typename TFalse >
	struct Conditional;

	template< typename TTrue, typename TFalse >
	struct Conditional< true, TTrue, TFalse > { using Type = TTrue; };

	template< typename TTrue, typename TFalse >
	struct Conditional< false, TTrue, TFalse > { using Type = TFalse; };
	
	template< bool Condition, typename TTrue, typename TFalse >
	using ConditionalT = typename Conditional<Condition, TTrue, TFalse >::Type;

	template< typename T, typename U >
	using LargerType = ConditionalT<(BitSize<T>::Value > BitSize<U>::Value), T, U>;

	template< typename T, typename U >
	using StrictLargerType = ConditionalT< (BitSize<T>::Value > BitSize<U>::Value), T, ConditionalT< (BitSize<U>::Value > BitSize<T>::Value), U, void > >;

	template< typename T, typename U >
	using SmallerType = ConditionalT<(BitSize<T>::Value < BitSize<U>::Value), T, U>;

	template< typename T, typename U >
	using StrictSmallerType = ConditionalT< (BitSize<T>::Value < BitSize<U>::Value), T, ConditionalT< (BitSize<U>::Value < BitSize<T>::Value), U, void > >;

	template< unsigned Bits, typename... Ts >
	struct LeastTypeHelper;

	template< unsigned Bits, typename T, typename... Ts >
	struct LeastTypeHelper<Bits, T, Ts... >
	{
		using Type = typename Conditional<(Bits <= BitSize<T>::Value), T, typename LeastTypeHelper<Bits, Ts...>::Type>::Type;
	};

	template< unsigned Bits >
	struct LeastTypeHelper<Bits>
	{
		using Type = void;
	};


	template< unsigned Bits, typename... Ts >
	using LeastType = typename LeastTypeHelper<Bits, Ts...>::Type;

	template< unsigned Bits >
	struct LeastUIntDef
	{
		static_assert(Bits <= BitSize<uintmax_t>::Value, "No type large enough");
		LeastUIntDef(void) = delete;
		using Type = typename Details::LeastType<Bits, uint_least8_t, uint_least16_t, uint_least32_t, uint_least64_t, uintmax_t>;
	};
	

	template< unsigned Bits >
	using LeastUInt = typename LeastUIntDef<Bits>::Type;

	template< unsigned Bits >
	struct LeastIntDef
	{
		static_assert(Bits <= BitSize<intmax_t>::Value, "No type large enough");
		LeastIntDef(void) = delete;
		using Type = typename Details::LeastType<Bits, int_least8_t, int_least16_t, int_least32_t, int_least64_t, intmax_t>;
	};

	template< unsigned Bits >
	using LeastInt = typename LeastIntDef<Bits>::Type;

	template< unsigned Bits >
	struct MsbMask
	{
		MsbMask(void) = delete;
		constexpr const static LeastUInt<Bits> Value = (1ull << (Bits - 1));
	};

	template< unsigned Bits >
	struct IdentityMask
	{
		IdentityMask(void) = delete;
		constexpr const static LeastUInt<Bits> Value = 1 | (IdentityMask<Bits - 1>::Value << 1);
	};

	template<>
	struct IdentityMask<0>
	{
		IdentityMask(void) = delete;
		constexpr const static LeastUInt<0> Value = 0;
	};



	using IntegerLiteral = decltype(0);
	using IntegerLiteralU = decltype(0U);
	using IntegerLiteralL = decltype(0L);
	using IntegerLiteralUL = decltype(0UL);
	using IntegerLiteralLL = decltype(0LL);
	using IntegerLiteralULL = decltype(0ULL);

	using DecimalLiteral = decltype(0.0);
	using DecimalLiteralF = decltype(0.0F);
	using DecimalLiteralL = decltype(0.0L);

	// Here be dragons!!
	//
	//
	//
	//  /\___/\  ~
	// ( O . O ) \\
	//  >  ^  <  //
	// ( \   / )//
	//  u U U u
	//
	//  Or cats?
	//  ~Mwrow~
}
END_FIXED_POINTS