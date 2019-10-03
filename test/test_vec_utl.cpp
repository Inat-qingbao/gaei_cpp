#include "ouchitest.hpp"
#include "vector_utl.hpp"

OUCHI_TEST_CASE(test_distance)
{
	{
		gaei::vector<int, 2> a{ 0, 0 };
		gaei::vector<int, 2> b{ 3, 0 };

        // distance(a, b) �� 3�Ɠ����������ׂ�B
		OUCHI_CHECK_EQUAL(gaei::distance(a, b), 3);
	}
	{
		gaei::vector<double, 3> a{ 0, 0, 0 };
		gaei::vector<double, 3> b{ 3, 0, 4 };

		OUCHI_CHECK_EQUAL(gaei::distance(a, b), 5);
    }
}

// �e�X�g�P�[�X���`����
OUCHI_TEST_CASE(test_innser_product)
{
	{
		gaei::vector<int, 2> a{ 3,4 };
		gaei::vector<int, 2> b{ 6,-5 };

		//inner_product(a,b)��-2�Ɠ����������ׂ�B
		OUCHI_CHECK_EQUAL(gaei::inner_product(a, b), -2);
	}
	{
		gaei::vector<int, 3> a{ 1,0,2 };
		gaei::vector<int, 3> b{ 2,1,2 };

		OUCHI_CHECK_EQUAL(gaei::inner_product(a, b), 6);
	}
}

OUCHI_TEST_CASE(test_cross_product)
{
	gaei::vector<int, 3> a{1 , 1, 1 };
	gaei::vector<int, 3> b{ 1, 1, -1 };
    auto ans = gaei::cross_product(a, b);
    OUCHI_CHECK_EQUAL(ans.x(), -2);
    OUCHI_CHECK_EQUAL(ans.y(), 2);
    OUCHI_CHECK_EQUAL(ans.z(), 0);

}
