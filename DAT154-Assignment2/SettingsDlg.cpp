#include "stdafx.h"
#include "SettingsDlg.h"

struct SettingsDialogData {
	trasim::simulator *sim;
};

std::wstring toPercentString(int num) {
	std::wstringstream ss;
	ss << num << "%";
	return ss.str();
}

void setupDialog(HWND hWnd, trasim::simulator *sim) {
	SetDlgItemText(hWnd, IDC_WPROB_READOUT, toPercentString(sim->west_spawn_rate()).c_str());
	SetDlgItemText(hWnd, IDC_NPROB_READOUT, toPercentString(sim->north_spawn_rate()).c_str());

	SendDlgItemMessage(hWnd, IDC_WPROB_SLIDER, TBM_SETRANGEMAX, false, 100);
	SendDlgItemMessage(hWnd, IDC_WPROB_SLIDER, TBM_SETRANGEMIN, false, 0);
	SendDlgItemMessage(hWnd, IDC_WPROB_SLIDER, TBM_SETPOS, true, sim->west_spawn_rate());

	SendDlgItemMessage(hWnd, IDC_NPROB_SLIDER, TBM_SETRANGEMAX, false, 100);
	SendDlgItemMessage(hWnd, IDC_NPROB_SLIDER, TBM_SETRANGEMIN, false, 0);
	SendDlgItemMessage(hWnd, IDC_NPROB_SLIDER, TBM_SETPOS, true, sim->north_spawn_rate());
}

void updateSimulation(HWND hWnd, trasim::simulator *sim) {
	auto wprob = SendDlgItemMessage(hWnd, IDC_WPROB_SLIDER, TBM_GETPOS, 0, 0);
	auto nprob = SendDlgItemMessage(hWnd, IDC_NPROB_SLIDER, TBM_GETPOS, 0, 0);
	SetDlgItemText(hWnd, IDC_WPROB_READOUT, toPercentString(sim->west_spawn_rate()).c_str());
	SetDlgItemText(hWnd, IDC_NPROB_READOUT, toPercentString(sim->north_spawn_rate()).c_str());

	sim->set_spawn_probabilities(wprob, nprob);
}

LRESULT CALLBACK SettingsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	SettingsDialogData *data = (SettingsDialogData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!data) {
		data = new SettingsDialogData();
	}

	switch (message) {
	case WM_INITDIALOG:
		TRACE(L"Init dialog\n");
		setupDialog(hWnd, (trasim::simulator*)lParam);
		data->sim = (trasim::simulator*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
		break;
	case WM_COMMAND:
		TRACE(L"Dialog command\n");
		if (LOWORD(wParam) == IDC_SETTINGS_OK) {
			EndDialog(hWnd, 0);
		}
		break;
	case WM_HSCROLL:
		updateSimulation(hWnd, data->sim);
		break;
	case WM_DESTROY:
		TRACE(L"Destroy settings dialog\n");
		if (data) {
			delete data;
			data = nullptr;
		}
		break;
	}

	return 0;
}
