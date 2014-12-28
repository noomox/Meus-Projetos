unit uClassesERP;

interface
   uses SysUtils,MinhasClasses,StrUtils;

   type
    TipoCampoChave = String;
    TTipoMovimento = (tmProduto,tmPatrimonio,tmEquipamento);

    TTIpoIPI = (iIPIAliq, iIPIValor);

    TTipoTransporte = (ttMaritima,ttFluvial,ttLacustre,ttAerea,ttPostal,ttFerroviaria,ttRodoviaria,ttConduto_Rede_Transmissao,ttMeios_Proprios,
                       ttEntrada_Ou_Saida_ficta);

    TTipoIntermedio= (tiImportacao_por_conta_propria,tiImportacao_por_conta_e_ordem,tiImportacao_por_encomenda);
    TTipoFrete = (tfEmitente,tfDestinatario, tfTerceiros);

    TRetorno = record
        Erro: Boolean;
        Mensagem: String;
    end;

    TTipoVerificacao = (tvCPF_CNPJ,tvIE,tvCEP,tvCFOP, tvUF, tvIBGE);

    TVerificacoes =  set of TTipoVerificacao;

    TTipoDocumento =(tdNFSe, tdNFe, tdComprovante);
    TTiposDocumento = set of TTipoDocumento;

    TTipoAmbiente = (tabHomologacao, tabProducao);



implementation

end.