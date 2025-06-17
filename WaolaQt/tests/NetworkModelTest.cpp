#include "MockINetwork.h"
#include "MockIPersistor.h"

#include "NetworkModelTestFixture.h"

using namespace testing;
using namespace Wpp;

void NetworkModelTestFixture::SetUp()
{
	net = std::make_unique<MockINetwork>();
	EXPECT_CALL(*net, init(_)).Times(1);

	persistor = std::make_unique<MockIPersistor>();
	EXPECT_CALL(*persistor, restore()).Times(1);
}

TEST_F(NetworkModelTestFixture, construct)
{
	EXPECT_CALL(*net, isEmpty()).WillOnce(Return(true));
	const ENetworkModel model(nullptr, std::move(net),
		std::move(persistor));
	ASSERT_TRUE(model.isEmpty());
}

TEST_F(NetworkModelTestFixture, rowCountEmpty)
{
	EXPECT_CALL(*net, hostCount()).WillOnce(Return(0));

	const ENetworkModel model(nullptr, std::move(net),
		std::move(persistor));
	ASSERT_EQ(model.rowCount(QModelIndex()), 0);
}

TEST_F(NetworkModelTestFixture, columnCount)
{
	const ENetworkModel model(nullptr, std::move(net),
		std::move(persistor));
	ASSERT_TRUE(model.columnCount(QModelIndex()) == arrsiz(model.columnNames));
}

TEST_F(NetworkModelTestFixture, addHostIncreasesRowCount)
{
#ifdef _WIN32
	EXPECT_CALL(*net, hostCount()).Times(5).WillOnce(Return(0)).WillOnce(Return(0))
		.WillOnce(Return(0)).WillOnce(Return(0)).WillOnce(Return(1));
#else
	EXPECT_CALL(*net, hostCount()).Times(4).WillOnce(Return(0)).WillOnce(Return(0))
		.WillOnce(Return(0)).WillOnce(Return(1));
#endif

	EXPECT_CALL(*net, addHost(_, _, _, _)).WillOnce(Return(true));

	ENetworkModel model(nullptr, std::move(net), std::move(persistor));;
	const int before = model.rowCount(QModelIndex());
	ASSERT_TRUE(model.addHost("TestHost", "test.local",
		"10.0.0.1", "00:11:22:33:44:55"));
	const int after = model.rowCount(QModelIndex());
	ASSERT_TRUE(after == before + 1);
}

TEST_F(NetworkModelTestFixture, wakeUpCallsNetworkWakeUp)
{
	EXPECT_CALL(*net, hostCount()).Times(2).WillOnce(Return(1)).WillOnce(Return(1));
	EXPECT_CALL(*net, wakeUp(Matcher<std::vector<int>&&>(_)));

	ENetworkModel model(nullptr, std::move(net), std::move(persistor));

	QList<QModelIndex> indexes;
	indexes << model.index(0, 0) << model.index(1, 0);

	ASSERT_NO_THROW(model.wakeUp(indexes));
}

TEST_F(NetworkModelTestFixture, removeHostDecreasesRowCount)
{
#ifdef _WIN32
	EXPECT_CALL(*net, hostCount()).Times(5).WillOnce(Return(1)).WillOnce(Return(1))
		.WillOnce(Return(1)).WillOnce(Return(1)).WillOnce(Return(0));
#else
	EXPECT_CALL(*net, hostCount()).Times(4).WillOnce(Return(1)).WillOnce(Return(1))
		.WillOnce(Return(1)).WillOnce(Return(0));
#endif

	EXPECT_CALL(*net, removeHosts(_, _));

	ENetworkModel model(nullptr, std::move(net), std::move(persistor));
	const int before = model.rowCount(QModelIndex());

	QList<QModelIndex> indexes;
	indexes << model.index(0, 0);
	ASSERT_NO_THROW(model.removeHosts(indexes));
	const int after = model.rowCount(QModelIndex());
	ASSERT_EQ(after, before - 1);
}
