#pragma once
namespace OldRequests
{
	struct Request
	{
		TSharedRef<IHttpRequest> request;
		std::function<void(TSharedRef<IHttpRequest>, bool)> callback;
		bool completed;
		bool remove_manually;

		bool operator==(const Request& rhs) const
		{
			return request == rhs.request;
		}
	};

	std::vector<Request> requests;
	bool CreateRequest(FString url,
		const std::function<void(TSharedRef<IHttpRequest>, bool)>& callback, FString verb = L"GET",
		const bool auto_remove = true, FString Postcontent = L"", FString header_value = L"text/html")
	{
		TSharedRef<IHttpRequest> request;
		FHttpModule::Get()->CreateRequest(&request);

		FString header_name = "Content-Type";
		FString Accepts_name = "Accepts";
		FString Accepts_value = "*/*";

		request->SetHeader(&header_name, &header_value);
		request->SetHeader(&Accepts_name, &Accepts_value);
		request->SetURL(&url);
		request->SetVerb(&verb);

		if(!Postcontent.IsEmpty()) request->SetContentAsString(&Postcontent);

		requests.push_back({ request, callback, false, !auto_remove });

		return request->ProcessRequest();
	}

	void RemoveRequest(const TSharedRef<IHttpRequest>& request)
	{
		requests.erase(remove_if(requests.begin(), requests.end(), [&request](const Request& cur_request)
		{
			return cur_request.request == request;
		}), requests.end());
	}

	void Update()
	{
		requests.erase(remove_if(requests.begin(), requests.end(), [](const Request& request)
		{
			return !request.remove_manually && request.completed;
		}), requests.end());

		const auto size = requests.size();
		for (auto i = 0; i < size; ++i)
		{
			auto& request = requests[i];

			const auto status = request.request->GetStatus();
			switch (status)
			{
			case EHttpRequestStatus::Succeeded:
			case EHttpRequestStatus::Failed:
				if (!request.completed)
				{
					request.completed = true;

					request.callback(request.request, status == EHttpRequestStatus::Succeeded);
				}
				break;
			case EHttpRequestStatus::NotStarted:
			case EHttpRequestStatus::Processing:
				break;
			default:;
			}
		}
	}
}