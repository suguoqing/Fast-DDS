// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file TestWriterPersistent.h
 *
 */

#ifndef TESTWRITERPERSISTENT_H_
#define TESTWRITERPERSISTENT_H_

#include <fastdds/rtps/writer/WriterListener.h>

namespace eprosima {
namespace fastdds {
namespace rtps {
class RTPSParticipant;
class WriterHistory;
class RTPSWriter;
} // namespace rtps
} // namespace fastdds
} // namespace eprosima

class TestWriterPersistent
{
public:

    TestWriterPersistent();
    virtual ~TestWriterPersistent();
    eprosima::fastdds::rtps::RTPSParticipant* mp_participant;
    eprosima::fastdds::rtps::RTPSWriter* mp_writer;
    eprosima::fastdds::rtps::WriterHistory* mp_history;
    bool init(); //Initialize writer
    bool reg(); //Register the Writer
    void run(
            uint16_t samples);  //Run the Writer
    class MyListener : public eprosima::fastdds::rtps::WriterListener
    {
    public:

        MyListener()
            : n_matched(0)
        {
        }

        ~MyListener()
        {
        }

        void onWriterMatched(
                eprosima::fastdds::rtps::RTPSWriter*,
                eprosima::fastdds::rtps::MatchingInfo& info) override
        {
            if (info.status == eprosima::fastdds::rtps::MATCHED_MATCHING)
            {
                ++n_matched;
            }
        }

        int n_matched;

    private:

        using eprosima::fastdds::rtps::WriterListener::onWriterMatched;
    }
    m_listener;
};

#endif /* TESTWRITERPERSISTENT_H_ */
